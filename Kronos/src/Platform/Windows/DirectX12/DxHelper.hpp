//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************
#ifndef __DX_HELPER_HPP__
#define __DX_HELPER_HPP__
#include <stdexcept>
#include "Windows/stdafx.hpp"

#pragma comment(lib, "dxguid.lib") // IID library

namespace Kronos {
	using Microsoft::WRL::ComPtr;
    inline std::string HrToString(HRESULT hr)
    {
        char s_str[64] = {};
        sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
        return std::string(s_str);
    }

    class HrException : public std::runtime_error
    {
    public:
        HrException(HRESULT hr) : std::runtime_error(HrToString(hr)), m_hr(hr) {}
        HRESULT Error() const { return m_hr; }
    private:
        const HRESULT m_hr;
    };

#define SAFE_RELEASE(p) if (p) (p)->Release()

    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw HrException(hr);
        }
    }

    inline void GetAssetsPath(_Out_writes_(pathSize) WCHAR* path, UINT pathSize)
    {
        if (path == nullptr)
        {
            throw std::exception();
        }

        DWORD size = GetModuleFileNameW(nullptr, path, pathSize);
        if (size == 0 || size == pathSize)
        {
            // Method failed or path was truncated.
            throw std::exception();
        }

        WCHAR* lastSlash = wcsrchr(path, L'\\');
        if (lastSlash)
        {
            *(lastSlash + 1) = L'\0';
        }
    }

    inline HRESULT ReadDataFromFile(LPCWSTR filename, byte** data, UINT* size)
    {
        using namespace Microsoft::WRL;

#if WINVER >= _WIN32_WINNT_WIN8
        CREATEFILE2_EXTENDED_PARAMETERS extendedParams = {};
        extendedParams.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
        extendedParams.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
        extendedParams.dwFileFlags = FILE_FLAG_SEQUENTIAL_SCAN;
        extendedParams.dwSecurityQosFlags = SECURITY_ANONYMOUS;
        extendedParams.lpSecurityAttributes = nullptr;
        extendedParams.hTemplateFile = nullptr;

        Wrappers::FileHandle file(CreateFile2(filename, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, &extendedParams));
#else
        Wrappers::FileHandle file(CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN | SECURITY_SQOS_PRESENT | SECURITY_ANONYMOUS, nullptr));
#endif
        if (file.Get() == INVALID_HANDLE_VALUE)
        {
            throw std::exception();
        }

        FILE_STANDARD_INFO fileInfo = {};
        if (!GetFileInformationByHandleEx(file.Get(), FileStandardInfo, &fileInfo, sizeof(fileInfo)))
        {
            throw std::exception();
        }

        if (fileInfo.EndOfFile.HighPart != 0)
        {
            throw std::exception();
        }

        *data = reinterpret_cast<byte*>(malloc(fileInfo.EndOfFile.LowPart));
        *size = fileInfo.EndOfFile.LowPart;

        if (!ReadFile(file.Get(), *data, fileInfo.EndOfFile.LowPart, nullptr, nullptr))
        {
            throw std::exception();
        }

        return S_OK;
    }

    inline HRESULT ReadDataFromDDSFile(LPCWSTR filename, byte** data, UINT* offset, UINT* size)
    {
        if (FAILED(ReadDataFromFile(filename, data, size)))
        {
            return E_FAIL;
        }

        // DDS files always start with the same magic number.
        static const UINT DDS_MAGIC = 0x20534444;
        UINT magicNumber = *reinterpret_cast<const UINT*>(*data);
        if (magicNumber != DDS_MAGIC)
        {
            return E_FAIL;
        }

        struct DDS_PIXELFORMAT
        {
            UINT size;
            UINT flags;
            UINT fourCC;
            UINT rgbBitCount;
            UINT rBitMask;
            UINT gBitMask;
            UINT bBitMask;
            UINT aBitMask;
        };

        struct DDS_HEADER
        {
            UINT size;
            UINT flags;
            UINT height;
            UINT width;
            UINT pitchOrLinearSize;
            UINT depth;
            UINT mipMapCount;
            UINT reserved1[11];
            DDS_PIXELFORMAT ddsPixelFormat;
            UINT caps;
            UINT caps2;
            UINT caps3;
            UINT caps4;
            UINT reserved2;
        };

        auto ddsHeader = reinterpret_cast<const DDS_HEADER*>(*data + sizeof(UINT));
        if (ddsHeader->size != sizeof(DDS_HEADER) || ddsHeader->ddsPixelFormat.size != sizeof(DDS_PIXELFORMAT))
        {
            return E_FAIL;
        }

        const ptrdiff_t ddsDataOffset = sizeof(UINT) + sizeof(DDS_HEADER);
        *offset = ddsDataOffset;
        *size = *size - ddsDataOffset;

        return S_OK;
    }

    // Assign a name to the object to aid with debugging.
#if defined(_DEBUG) || defined(DBG)
    inline void SetName(ID3D12Object* pObject, LPCWSTR name)
    {
        pObject->SetName(name);
    }
    inline void SetNameIndexed(ID3D12Object* pObject, LPCWSTR name, UINT index)
    {
        WCHAR fullName[50];
        if (swprintf_s(fullName, L"%s[%u]", name, index) > 0)
        {
            pObject->SetName(fullName);
        }
    }
#else
    inline void SetName(ID3D12Object*, LPCWSTR)
    {
    }
    inline void SetNameIndexed(ID3D12Object*, LPCWSTR, UINT)
    {
    }
#endif

    // Naming helper for ComPtr<T>.
    // Assigns the name of the variable as the name of the object.
    // The indexed variant will include the index in the name of the object.
#define NAME_D3D12_OBJECT(x) SetName((x).Get(), L#x)
#define NAME_D3D12_OBJECT_INDEXED(x, n) SetNameIndexed((x)[n].Get(), L#x, n)

    inline UINT CalculateConstantBufferByteSize(UINT byteSize)
    {
        // Constant buffer size is required to be aligned.
        return (byteSize + (D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1)) & ~(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1);
    }

#ifdef D3D_COMPILE_STANDARD_FILE_INCLUDE
    inline Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(
        const std::wstring& filename,
        const D3D_SHADER_MACRO* defines,
        const std::string& entrypoint,
        const std::string& target)
    {
        UINT compileFlags = 0;
#if defined(_DEBUG) || defined(DBG)
        compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

        HRESULT hr;

        Microsoft::WRL::ComPtr<ID3DBlob> byteCode = nullptr;
        Microsoft::WRL::ComPtr<ID3DBlob> errors;
        hr = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entrypoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);

        if (errors != nullptr)
        {
            OutputDebugStringA((char*)errors->GetBufferPointer());
        }
        ThrowIfFailed(hr);

        return byteCode;
    }
#endif

    // Resets all elements in a ComPtr array.
    template<class T>
    void ResetComPtrArray(T* comPtrArray)
    {
        for (auto& i : *comPtrArray)
        {
            i.Reset();
        }
    }


    // Resets all elements in a unique_ptr array.
    template<class T>
    void ResetUniquePtrArray(T* uniquePtrArray)
    {
        for (auto& i : *uniquePtrArray)
        {
            i.reset();
        }
    }

    //------------------------------------------------------------------------------------------------
    // D3D12 exports a new method for serializing root signatures in the Windows 10 Anniversary Update.
    // To help enable root signature 1.1 features when they are available and not require maintaining
    // two code paths for building root signatures, this helper method reconstructs a 1.0 signature when
    // 1.1 is not supported.
    inline HRESULT D3DX12SerializeVersionedRootSignature(
        _In_ const D3D12_VERSIONED_ROOT_SIGNATURE_DESC* pRootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION MaxVersion,
        _Outptr_ ID3DBlob** ppBlob,
        _Always_(_Outptr_opt_result_maybenull_) ID3DBlob** ppErrorBlob) noexcept
    {
        if (ppErrorBlob != nullptr)
        {
            *ppErrorBlob = nullptr;
        }

        switch (MaxVersion)
        {
        case D3D_ROOT_SIGNATURE_VERSION_1_0:
            switch (pRootSignatureDesc->Version)
            {
            case D3D_ROOT_SIGNATURE_VERSION_1_0:
                return D3D12SerializeRootSignature(&pRootSignatureDesc->Desc_1_0, D3D_ROOT_SIGNATURE_VERSION_1, ppBlob, ppErrorBlob);

            case D3D_ROOT_SIGNATURE_VERSION_1_1:
            {
                HRESULT hr = S_OK;
                const D3D12_ROOT_SIGNATURE_DESC1& desc_1_1 = pRootSignatureDesc->Desc_1_1;

                const SIZE_T ParametersSize = sizeof(D3D12_ROOT_PARAMETER) * desc_1_1.NumParameters;
                void* pParameters = (ParametersSize > 0) ? HeapAlloc(GetProcessHeap(), 0, ParametersSize) : nullptr;
                if (ParametersSize > 0 && pParameters == nullptr)
                {
                    hr = E_OUTOFMEMORY;
                }
                auto pParameters_1_0 = static_cast<D3D12_ROOT_PARAMETER*>(pParameters);

                if (SUCCEEDED(hr))
                {
                    for (UINT n = 0; n < desc_1_1.NumParameters; n++)
                    {
                        __analysis_assume(ParametersSize == sizeof(D3D12_ROOT_PARAMETER) * desc_1_1.NumParameters);
                        pParameters_1_0[n].ParameterType = desc_1_1.pParameters[n].ParameterType;
                        pParameters_1_0[n].ShaderVisibility = desc_1_1.pParameters[n].ShaderVisibility;

                        switch (desc_1_1.pParameters[n].ParameterType)
                        {
                        case D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS:
                            pParameters_1_0[n].Constants.Num32BitValues = desc_1_1.pParameters[n].Constants.Num32BitValues;
                            pParameters_1_0[n].Constants.RegisterSpace = desc_1_1.pParameters[n].Constants.RegisterSpace;
                            pParameters_1_0[n].Constants.ShaderRegister = desc_1_1.pParameters[n].Constants.ShaderRegister;
                            break;

                        case D3D12_ROOT_PARAMETER_TYPE_CBV:
                        case D3D12_ROOT_PARAMETER_TYPE_SRV:
                        case D3D12_ROOT_PARAMETER_TYPE_UAV:
                            pParameters_1_0[n].Descriptor.RegisterSpace = desc_1_1.pParameters[n].Descriptor.RegisterSpace;
                            pParameters_1_0[n].Descriptor.ShaderRegister = desc_1_1.pParameters[n].Descriptor.ShaderRegister;
                            break;

                        case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
                            const D3D12_ROOT_DESCRIPTOR_TABLE1& table_1_1 = desc_1_1.pParameters[n].DescriptorTable;

                            const SIZE_T DescriptorRangesSize = sizeof(D3D12_DESCRIPTOR_RANGE) * table_1_1.NumDescriptorRanges;
                            void* pDescriptorRanges = (DescriptorRangesSize > 0 && SUCCEEDED(hr)) ? HeapAlloc(GetProcessHeap(), 0, DescriptorRangesSize) : nullptr;
                            if (DescriptorRangesSize > 0 && pDescriptorRanges == nullptr)
                            {
                                hr = E_OUTOFMEMORY;
                            }
                            auto pDescriptorRanges_1_0 = static_cast<D3D12_DESCRIPTOR_RANGE*>(pDescriptorRanges);

                            if (SUCCEEDED(hr))
                            {
                                for (UINT x = 0; x < table_1_1.NumDescriptorRanges; x++)
                                {
                                    __analysis_assume(DescriptorRangesSize == sizeof(D3D12_DESCRIPTOR_RANGE) * table_1_1.NumDescriptorRanges);
                                    pDescriptorRanges_1_0[x].BaseShaderRegister = table_1_1.pDescriptorRanges[x].BaseShaderRegister;
                                    pDescriptorRanges_1_0[x].NumDescriptors = table_1_1.pDescriptorRanges[x].NumDescriptors;
                                    pDescriptorRanges_1_0[x].OffsetInDescriptorsFromTableStart = table_1_1.pDescriptorRanges[x].OffsetInDescriptorsFromTableStart;
                                    pDescriptorRanges_1_0[x].RangeType = table_1_1.pDescriptorRanges[x].RangeType;
                                    pDescriptorRanges_1_0[x].RegisterSpace = table_1_1.pDescriptorRanges[x].RegisterSpace;
                                }
                            }

                            D3D12_ROOT_DESCRIPTOR_TABLE& table_1_0 = pParameters_1_0[n].DescriptorTable;
                            table_1_0.NumDescriptorRanges = table_1_1.NumDescriptorRanges;
                            table_1_0.pDescriptorRanges = pDescriptorRanges_1_0;
                        }
                    }
                }

                if (SUCCEEDED(hr))
                {
                    CD3DX12_ROOT_SIGNATURE_DESC desc_1_0(desc_1_1.NumParameters, pParameters_1_0, desc_1_1.NumStaticSamplers, desc_1_1.pStaticSamplers, desc_1_1.Flags);
                    hr = D3D12SerializeRootSignature(&desc_1_0, D3D_ROOT_SIGNATURE_VERSION_1, ppBlob, ppErrorBlob);
                }

                if (pParameters)
                {
                    for (UINT n = 0; n < desc_1_1.NumParameters; n++)
                    {
                        if (desc_1_1.pParameters[n].ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
                        {
                            HeapFree(GetProcessHeap(), 0, reinterpret_cast<void*>(const_cast<D3D12_DESCRIPTOR_RANGE*>(pParameters_1_0[n].DescriptorTable.pDescriptorRanges)));
                        }
                    }
                    HeapFree(GetProcessHeap(), 0, pParameters);
                }
                return hr;
            }
            }
            break;

        case D3D_ROOT_SIGNATURE_VERSION_1_1:
            return D3D12SerializeVersionedRootSignature(pRootSignatureDesc, ppBlob, ppErrorBlob);
        }

        return E_INVALIDARG;
    }

    // Returns required size of a buffer to be used for data upload
    inline UINT64 GetRequiredIntermediateSize(
        _In_ ID3D12Resource* pDestinationResource,
        _In_range_(0, D3D12_REQ_SUBRESOURCES) UINT FirstSubresource,
        _In_range_(0, D3D12_REQ_SUBRESOURCES - FirstSubresource) UINT NumSubresources) noexcept
    {
        auto Desc = pDestinationResource->GetDesc();
        UINT64 RequiredSize = 0;

        ID3D12Device* pDevice = nullptr;
        pDestinationResource->GetDevice(IID_ID3D12Device, reinterpret_cast<void**>(&pDevice));
        pDevice->GetCopyableFootprints(&Desc, FirstSubresource, NumSubresources, 0, nullptr, nullptr, nullptr, &RequiredSize);
        pDevice->Release();

        return RequiredSize;
    }
    // Row-by-row memcpy
    inline void MemcpySubresource(
        _In_ const D3D12_MEMCPY_DEST* pDest,
        _In_ const D3D12_SUBRESOURCE_DATA* pSrc,
        SIZE_T RowSizeInBytes,
        UINT NumRows,
        UINT NumSlices) noexcept
    {
        for (UINT z = 0; z < NumSlices; ++z)
        {
            auto pDestSlice = static_cast<BYTE*>(pDest->pData) + pDest->SlicePitch * z;
            auto pSrcSlice = static_cast<const BYTE*>(pSrc->pData) + pSrc->SlicePitch * LONG_PTR(z);
            for (UINT y = 0; y < NumRows; ++y)
            {
                memcpy(pDestSlice + pDest->RowPitch * y,
                    pSrcSlice + pSrc->RowPitch * LONG_PTR(y),
                    RowSizeInBytes);
            }
        }
    }
    //------------------------------------------------------------------------------------------------
    // All arrays must be populated (e.g. by calling GetCopyableFootprints)
    inline UINT64 UpdateSubresources(
        _In_ ID3D12GraphicsCommandList* pCmdList,
        _In_ ID3D12Resource* pDestinationResource,
        _In_ ID3D12Resource* pIntermediate,
        _In_range_(0, D3D12_REQ_SUBRESOURCES) UINT FirstSubresource,
        _In_range_(0, D3D12_REQ_SUBRESOURCES - FirstSubresource) UINT NumSubresources,
        UINT64 RequiredSize,
        _In_reads_(NumSubresources) const D3D12_PLACED_SUBRESOURCE_FOOTPRINT* pLayouts,
        _In_reads_(NumSubresources) const UINT* pNumRows,
        _In_reads_(NumSubresources) const UINT64* pRowSizesInBytes,
        _In_reads_(NumSubresources) const D3D12_SUBRESOURCE_DATA* pSrcData) noexcept
    {
        // Minor validation
        auto IntermediateDesc = pIntermediate->GetDesc();
        auto DestinationDesc = pDestinationResource->GetDesc();
        if (IntermediateDesc.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER ||
            IntermediateDesc.Width < RequiredSize + pLayouts[0].Offset ||
            RequiredSize > SIZE_T(-1) ||
            (DestinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER &&
                (FirstSubresource != 0 || NumSubresources != 1)))
        {
            return 0;
        }

        BYTE* pData;
        HRESULT hr = pIntermediate->Map(0, nullptr, reinterpret_cast<void**>(&pData));
        if (FAILED(hr))
        {
            return 0;
        }

        for (UINT i = 0; i < NumSubresources; ++i)
        {
            if (pRowSizesInBytes[i] > SIZE_T(-1)) return 0;
            D3D12_MEMCPY_DEST DestData = { pData + pLayouts[i].Offset, pLayouts[i].Footprint.RowPitch, SIZE_T(pLayouts[i].Footprint.RowPitch) * SIZE_T(pNumRows[i]) };
            MemcpySubresource(&DestData, &pSrcData[i], static_cast<SIZE_T>(pRowSizesInBytes[i]), pNumRows[i], pLayouts[i].Footprint.Depth);
        }
        pIntermediate->Unmap(0, nullptr);

        if (DestinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
        {
            pCmdList->CopyBufferRegion(
                pDestinationResource, 0, pIntermediate, pLayouts[0].Offset, pLayouts[0].Footprint.Width);
        }
        else
        {
            for (UINT i = 0; i < NumSubresources; ++i)
            {
                CD3DX12_TEXTURE_COPY_LOCATION Dst(pDestinationResource, i + FirstSubresource);
                CD3DX12_TEXTURE_COPY_LOCATION Src(pIntermediate, pLayouts[i]);
                pCmdList->CopyTextureRegion(&Dst, 0, 0, 0, &Src, nullptr);
            }
        }
        return RequiredSize;
    }

    //------------------------------------------------------------------------------------------------
    // Heap-allocating UpdateSubresources implementation
    inline UINT64 UpdateSubresources(
        _In_ ID3D12GraphicsCommandList* pCmdList,
        _In_ ID3D12Resource* pDestinationResource,
        _In_ ID3D12Resource* pIntermediate,
        UINT64 IntermediateOffset,
        _In_range_(0, D3D12_REQ_SUBRESOURCES) UINT FirstSubresource,
        _In_range_(0, D3D12_REQ_SUBRESOURCES - FirstSubresource) UINT NumSubresources,
        _In_reads_(NumSubresources) const D3D12_SUBRESOURCE_DATA* pSrcData) noexcept
    {
        UINT64 RequiredSize = 0;
        UINT64 MemToAlloc = static_cast<UINT64>(sizeof(D3D12_PLACED_SUBRESOURCE_FOOTPRINT) + sizeof(UINT) + sizeof(UINT64)) * NumSubresources;
        if (MemToAlloc > SIZE_MAX)
        {
            return 0;
        }
        void* pMem = HeapAlloc(GetProcessHeap(), 0, static_cast<SIZE_T>(MemToAlloc));
        if (pMem == nullptr)
        {
            return 0;
        }
        auto pLayouts = static_cast<D3D12_PLACED_SUBRESOURCE_FOOTPRINT*>(pMem);
        UINT64* pRowSizesInBytes = reinterpret_cast<UINT64*>(pLayouts + NumSubresources);
        UINT* pNumRows = reinterpret_cast<UINT*>(pRowSizesInBytes + NumSubresources);

        auto Desc = pDestinationResource->GetDesc();
        ID3D12Device* pDevice = nullptr;
        pDestinationResource->GetDevice(IID_ID3D12Device, reinterpret_cast<void**>(&pDevice));
        pDevice->GetCopyableFootprints(&Desc, FirstSubresource, NumSubresources, IntermediateOffset, pLayouts, pNumRows, pRowSizesInBytes, &RequiredSize);
        pDevice->Release();

        UINT64 Result = UpdateSubresources(pCmdList, pDestinationResource, pIntermediate, FirstSubresource, NumSubresources, RequiredSize, pLayouts, pNumRows, pRowSizesInBytes, pSrcData);
        HeapFree(GetProcessHeap(), 0, pMem);
        return Result;
    }

    //------------------------------------------------------------------------------------------------
    // Stack-allocating UpdateSubresources implementation
    template <UINT MaxSubresources>
    inline UINT64 UpdateSubresources(
        _In_ ID3D12GraphicsCommandList* pCmdList,
        _In_ ID3D12Resource* pDestinationResource,
        _In_ ID3D12Resource* pIntermediate,
        UINT64 IntermediateOffset,
        _In_range_(0, MaxSubresources) UINT FirstSubresource,
        _In_range_(1, MaxSubresources - FirstSubresource) UINT NumSubresources,
        _In_reads_(NumSubresources) const D3D12_SUBRESOURCE_DATA* pSrcData) noexcept
    {
        UINT64 RequiredSize = 0;
        D3D12_PLACED_SUBRESOURCE_FOOTPRINT Layouts[MaxSubresources];
        UINT NumRows[MaxSubresources];
        UINT64 RowSizesInBytes[MaxSubresources];

        auto Desc = pDestinationResource->GetDesc();
        ID3D12Device* pDevice = nullptr;
        pDestinationResource->GetDevice(IID_ID3D12Device, reinterpret_cast<void**>(&pDevice));
        pDevice->GetCopyableFootprints(&Desc, FirstSubresource, NumSubresources, IntermediateOffset, Layouts, NumRows, RowSizesInBytes, &RequiredSize);
        pDevice->Release();

        return UpdateSubresources(pCmdList, pDestinationResource, pIntermediate, FirstSubresource, NumSubresources, RequiredSize, Layouts, NumRows, RowSizesInBytes, pSrcData);
    }
}
#endif