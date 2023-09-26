#include "stdafx.h"

#include <memory>
#include <DirectXTex.h>

using namespace DirectX;

void fix_texture_name(LPSTR fn) {
    auto _ext = strext(fn);
    if (_ext &&
        (0 == _stricmp(_ext, ".tga") ||
            0 == _stricmp(_ext, ".dds") ||
            0 == _stricmp(_ext, ".bmp") ||
            0 == _stricmp(_ext, ".ogm")))
        *_ext = 0;
}

int get_texture_load_lod(LPCSTR fn) {
    auto& sect = pSettings->r_section("reduce_lod_texture_list");

    for (const auto& data : sect.Data) {
        if (strstr(fn, data.first.c_str())) {
            if (psTextureLOD < 1) {
                return 0;
            } else {
                if (psTextureLOD < 3) {
                    return 1;
                } else {
                    return 2;
                }
            }
        }
    }

    if (psTextureLOD < 2) {
        return 0;
    } else {
        if (psTextureLOD < 4) {
            return 1;
        } else {
            return 2;
        }
    }
}

u32 calc_texture_size(int lod, u32 mip_cnt, u32 orig_size) {
    if (1 == mip_cnt) {
        return orig_size;
    }
    int _lod = lod;
    float res = float(orig_size);

    while (_lod > 0) {
        --_lod;
        res -= res / 1.333f;
    }
    return iFloor(res);
}

IC void	Reduce(size_t& w, size_t& h, size_t& l, int& skip) {
    while ((l > 1) && skip) {
        w /= 2;
        h /= 2;
        l -= 1;

        skip--;
    }
    if (w < 1) {
        w = 1;
    }
    if (h < 1) {
        h = 1;
    }
}

ID3DBaseTexture* CRender::texture_load(LPCSTR fRName, u32& ret_msize, bool bStaging) {
    // Moved here just to avoid warning
    TexMetadata imageInfo{};

    // Staging control
    static bool bAllowStaging = !RImplementation.o.no_ram_textures;
    bStaging &= bAllowStaging;

    ID3DBaseTexture* pTexture2D = nullptr;
    string_path fn;
    u32 img_size = 0;
    int img_loaded_lod = 0;
    u32 mip_cnt = u32(-1);
    // validation
    R_ASSERT(fRName);
    R_ASSERT(fRName[0]);

    // make file name
    string_path fname;
    xr_strcpy(fname, fRName);
    fix_texture_name(fname);
    IReader* reader = nullptr;
    if (!FS.exist(fn, "$game_textures$", fname, ".dds") && strstr(fname, "_bump")) {
        goto _BUMP_from_base;
    }
    if (FS.exist(fn, "$level$", fname, ".dds")) {
        goto _DDS;
    }
    if (FS.exist(fn, "$game_saves$", fname, ".dds")) {
        goto _DDS;
    }
    if (FS.exist(fn, "$game_textures$", fname, ".dds")) {
        goto _DDS;
    }

#ifdef _EDITOR
    ELog.Msg(mtError, "Can't find texture '%s'", fname);
    return 0;
#else

    Msg("! Can't find texture '%s'", fname);
    R_ASSERT(FS.exist(fn, "$game_textures$", "ed\\ed_not_existing_texture", ".dds"));
    goto _DDS;

#endif

    _DDS: {
        // Load and get header
        reader = FS.r_open(fn);
#ifdef DEBUG
        Msg("* Loaded: %s[%d]b", fn, reader->length());
#endif // DEBUG
        img_size = reader->length();
        R_ASSERT(reader);
        R_CHK2(GetMetadataFromDDSMemory(reader->pointer(), reader->length(), DDS_FLAGS::DDS_FLAGS_NONE, imageInfo), fn);
        if (imageInfo.IsCubemap() || imageInfo.IsVolumemap()) {
            goto _DDS_CUBE;
        } else {
            goto _DDS_2D;
        }
    _DDS_CUBE: {
        auto scratchImage = std::make_unique<ScratchImage>();
        HRESULT hr = LoadFromDDSMemory(reader->pointer(), reader->length(), DDS_FLAGS::DDS_FLAGS_NONE, &imageInfo, *scratchImage);
        auto usage = (bStaging) ? D3D_USAGE_STAGING : D3D_USAGE_DEFAULT;
        auto bindFlags = (bStaging) ? 0 : D3D_BIND_SHADER_RESOURCE;
        auto cpuAccessFlags = (bStaging) ? D3D_CPU_ACCESS_WRITE : 0;
        auto miscFlags = imageInfo.miscFlags;
        
        hr = CreateTextureEx(RCache.get_Device(), scratchImage->GetImages(), scratchImage->GetImageCount(),
            imageInfo, usage, bindFlags, cpuAccessFlags, miscFlags, CREATETEX_FLAGS::CREATETEX_DEFAULT, &pTexture2D);

        FS.r_close(reader);
        mip_cnt = imageInfo.mipLevels;
        ret_msize = calc_texture_size(img_loaded_lod, mip_cnt, img_size);
        return pTexture2D;
        }
    _DDS_2D: {
        // Check for LMAP and compress if needed
        _strlwr(fn);

        img_loaded_lod = get_texture_load_lod(fn);

        auto scratchImage = std::make_unique<ScratchImage>();
        HRESULT hr = LoadFromDDSMemory(reader->pointer(), reader->length(), DDS_FLAGS::DDS_FLAGS_NONE, &imageInfo, *scratchImage);
        
        auto usage = (bStaging) ? D3D_USAGE_STAGING : D3D_USAGE_DEFAULT;
        auto bindFlags = (bStaging) ? 0 : D3D_BIND_SHADER_RESOURCE;
        auto cpuAccessFlags = (bStaging) ? D3D_CPU_ACCESS_WRITE : 0;
        auto miscFlags = imageInfo.miscFlags;
        int old_mipmap_cnt = 0, mip_lod = 0;
        if (img_loaded_lod) {
            old_mipmap_cnt = imageInfo.mipLevels;
            Reduce(imageInfo.width, imageInfo.height, imageInfo.mipLevels, img_loaded_lod);
            mip_lod = old_mipmap_cnt - imageInfo.mipLevels;
        }

        hr = CreateTextureEx(RCache.get_Device(), scratchImage->GetImages() + mip_lod, scratchImage->GetImageCount(),
            imageInfo, usage, bindFlags, cpuAccessFlags, miscFlags, CREATETEX_FLAGS::CREATETEX_DEFAULT, &pTexture2D);
        FS.r_close(reader);
        
        mip_cnt = imageInfo.mipLevels;
        ret_msize = calc_texture_size(img_loaded_lod, mip_cnt, img_size);
        return pTexture2D;
    }
}
    _BUMP_from_base: {
        Msg("! Fallback to default bump map: %s", fname);
        if (strstr(fname, "_bump#")) {
            R_ASSERT2(FS.exist(fn, "$game_textures$", "ed\\ed_dummy_bump#", ".dds"), "ed_dummy_bump#");
            reader = FS.r_open(fn);
            R_ASSERT2(reader, fn);
            img_size = reader->length();
            goto _DDS_2D;
        }
        if (strstr(fname, "_bump")) {
            R_ASSERT2(FS.exist(fn, "$game_textures$", "ed\\ed_dummy_bump", ".dds"), "ed_dummy_bump");
            reader = FS.r_open(fn);
            R_ASSERT2(reader, fn);
            img_size = reader->length();
            goto _DDS_2D;
        }
    }

    return nullptr;
}
