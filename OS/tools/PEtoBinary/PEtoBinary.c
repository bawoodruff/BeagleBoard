#include <windows.h>
#include <imagehlp.h>
#include <process.h>
#include <stdio.h>

ULONG RvaToAbsoluteOffset(
   PVOID Base,
   ULONG Rva
   )
{
	USHORT i;
	ULONG absoluteOffset = 0;
	PIMAGE_NT_HEADERS ntHeaders = ImageNtHeader(Base);
	PIMAGE_SECTION_HEADER sectionHeader = IMAGE_FIRST_SECTION(ntHeaders);

	
	for (i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++)
	{
		if ((Rva >= sectionHeader->VirtualAddress) && (Rva < (sectionHeader->VirtualAddress + sectionHeader->SizeOfRawData)))
		{
			absoluteOffset += (Rva - sectionHeader->VirtualAddress);
			break;
		}

		if ((0 == strncmp(sectionHeader->Name, ".text", 5)) ||
			(0 == strncmp(sectionHeader->Name, ".rdata", 6)) || 
			(0 == strncmp(sectionHeader->Name, ".data", 5)))
		{
			absoluteOffset += sectionHeader->SizeOfRawData;
			sectionHeader++;
		}
	}

	return absoluteOffset;
}

HRESULT FixupSection(
   PVOID Base,
   ULONG RomImageBase,
   ULONG Va,
   ULONG RawData,
   ULONG SizeInBytes)
{
	PIMAGE_SECTION_HEADER sectionHeader;
	PIMAGE_BASE_RELOCATION relocEntry;
	PULONG fixupLocation;
	PUSHORT relocation;
	ULONG relocBlockCount;
	ULONG relocSize;
	USHORT i;

	HRESULT hr = S_OK;

	relocEntry = ImageDirectoryEntryToDataEx(Base, FALSE, IMAGE_DIRECTORY_ENTRY_BASERELOC, &relocSize, &sectionHeader);

	while (relocSize && SUCCEEDED(hr))
	{
		relocBlockCount = (relocEntry->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
		relocation = (PUSHORT)(relocEntry + 1);

		if (((ULONG) relocEntry->VirtualAddress >= Va) && (relocEntry->VirtualAddress < (Va + SizeInBytes)))
		{
			ULONG Rva;
			ULONG Masked;

			//printf("Virtual Address:%x, Size: %x (Count: %d)\n", relocEntry->VirtualAddress, relocEntry->SizeOfBlock, relocBlockCount);

			for (i = 0; i < relocBlockCount && SUCCEEDED(hr); i++)
			{
				//printf("\t%08x, type: %d\n", relocEntry->VirtualAddress + (relocation[i] & 0xFFF), (relocation[i] & 0xF000) >> 12);

				fixupLocation = (PULONG)((ULONG) RawData + (relocEntry->VirtualAddress - Va) + (relocation[i] & 0xFFF));

				switch ((relocation[i] &0xF000) >> 12)
				{
					case IMAGE_REL_BASED_HIGHLOW:
						//printf("\t\trelocation @ %x, Rva = %x, offset = %x\n", fixupLocation, *fixupLocation, 
						//       RomImageBase + RvaToAbsoluteOffset(Base, *fixupLocation ));

						*fixupLocation = RomImageBase + RvaToAbsoluteOffset(Base, *fixupLocation);

						break;

					case IMAGE_REL_BASED_ARM_MOV32T:

						// First, the lower 16 w/ MOVT

						Rva = MAKELONG(((PUSHORT) fixupLocation)[1], ((PUSHORT) fixupLocation)[0]);

						Masked = Rva & 0xFBF08F00;

						Rva = ((Rva & 0x000F0000) >> 4) | ((Rva & 0x04000000) >> 15) | ((Rva & 0x00000700) >> 4) | (Rva & 0x000000FF);

						//printf("\t\trelocation @ %x, Rva = %x, offset = %x\n", fixupLocation, Rva,
						//	   RomImageBase + RvaToAbsoluteOffset(Base, Rva));

						Rva = RomImageBase + RvaToAbsoluteOffset(Base, Rva);
						Masked |= ((Rva & 0x0000F000) << 4) | ((Rva & 0x00000800) << 15)  | ((Rva & 0x00000700) << 4) | (Rva & 0x000000FF);

						*fixupLocation++;

						// Now do the upper 16 w/ MOVT

						Masked = (MAKELONG(((PUSHORT) fixupLocation)[1], ((PUSHORT) fixupLocation)[0])) & 0xFBF08F00;

						Masked |= ((Rva & 0xF0000000) >> 12) | ((Rva & 0x00800000) << 3)  | ((Rva & 0x00700000) >> 12) | ((Rva & 0x00FF0000) >> 16);

						*fixupLocation = Masked;

						break;

					case IMAGE_REL_BASED_ABSOLUTE:

						// No change needed

						break;

					default:
						printf("ERROR: Unknown relocation type in image (%d)\n", (relocation[i] &0xF000) >> 12);
						hr = HRESULT_FROM_WIN32(ERROR_INVALID_DATA);

						break;
				}
			}
		}

		if (FAILED(hr))
		{
			break;
		}

		relocSize -= relocEntry->SizeOfBlock;

		(PUCHAR) relocEntry += relocEntry->SizeOfBlock;
	}

	return hr;
}

int __cdecl wmain(int argc, wchar_t* argv[])
{
	HANDLE inFile, outFile, fileMapping;
	PVOID mappedFile;

	ULONG romImageBase = 0x40200800;
	HRESULT hr = S_OK;

	if (argc < 3)
	{
		printf("Usage:\n\tPEtoBinary <filein> <fileout>\n");
		hr = HRESULT_FROM_WIN32(ERROR_BAD_ARGUMENTS);
	}
	else
	{
		fileMapping = NULL;

		inFile = CreateFile(argv[1], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);

		outFile = CreateFile(argv[2], GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);

		if (INVALID_HANDLE_VALUE == inFile)
		{
			printf("ERROR: file not found (%S)\n", argv[1]);
			hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
		}
		else if (INVALID_HANDLE_VALUE == outFile)
		{
			printf("ERROR: unabled to create output file (%S)\n", argv[2]);
			hr = HRESULT_FROM_WIN32(GetLastError());
		}
		else
		{
			fileMapping = CreateFileMapping(inFile, NULL, PAGE_WRITECOPY, 0, 0, NULL);

			if (NULL == fileMapping)
			{
				printf("ERROR: failed to map the file (%d)\n", GetLastError());
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			else
			{
				mappedFile = MapViewOfFile(fileMapping, FILE_MAP_COPY, 0, 0, 0);

				if (mappedFile)
				{
					PIMAGE_NT_HEADERS ntHeaders;
					PIMAGE_OPTIONAL_HEADER32 optionalHeader;
					PIMAGE_SECTION_HEADER sectionHeader;
					USHORT i;

					ntHeaders = ImageNtHeader(mappedFile);

					if (ntHeaders->Signature == IMAGE_NT_SIGNATURE)
					{
						printf("Verified header signature is a PE signature.\n");
					}

					optionalHeader = (PIMAGE_OPTIONAL_HEADER32)&ntHeaders->OptionalHeader;
					printf("Image base = %x\n", optionalHeader->ImageBase);
					printf("Entry point = %x\n", optionalHeader->AddressOfEntryPoint);
					printf("Found %d sections\n", ntHeaders->FileHeader.NumberOfSections);

					sectionHeader = IMAGE_FIRST_SECTION(ntHeaders);
					for (i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++)
					{
						if ((0 == strncmp(sectionHeader->Name, ".text", 5)) ||
							(0 == strncmp(sectionHeader->Name, ".rdata", 6)) || 
							(0 == strncmp(sectionHeader->Name, ".data", 5)))
						{
							ULONG bytesWritten;

							printf("Section #%d, Name = %s\n", i, sectionHeader->Name);
							printf("\tVirtualAddress = %x\n", sectionHeader->VirtualAddress);
							printf("\tSizeOfRawData = %x\n", sectionHeader->SizeOfRawData);
							printf("\tPointerToRawData = %x\n", sectionHeader->PointerToRawData);
							printf("\tCharacteristics = %x\n\n", sectionHeader->Characteristics);

							hr = FixupSection(mappedFile, romImageBase, sectionHeader->VirtualAddress, (ULONG) mappedFile + sectionHeader->PointerToRawData, sectionHeader->SizeOfRawData);
							if (FAILED(hr))
							{
								printf("ERROR: unable to fixup image, error = %08x\n", hr);
								break;
							}

							printf("Writing section %s of %d bytes\n", sectionHeader->Name, sectionHeader->SizeOfRawData);
							WriteFile(outFile, (PUCHAR) mappedFile + sectionHeader->PointerToRawData, sectionHeader->SizeOfRawData, &bytesWritten, NULL );
							if (bytesWritten != sectionHeader->SizeOfRawData)
							{
								printf("ERROR: failed to write section\n");
							}
						}
						sectionHeader++;
					}

					UnmapViewOfFile(mappedFile);
				}
				else
				{
					printf("ERROR: failed to map view of file (%d)\n", GetLastError());
					hr = HRESULT_FROM_WIN32(GetLastError());
				}

			}
		}

		if (NULL != fileMapping)
		{
			CloseHandle(fileMapping);
		}
		if (INVALID_HANDLE_VALUE != inFile)
		{
			CloseHandle(inFile);
		}

		if (INVALID_HANDLE_VALUE != outFile)
		{
			CloseHandle(outFile);
		}
	}

	exit(hr);
}
