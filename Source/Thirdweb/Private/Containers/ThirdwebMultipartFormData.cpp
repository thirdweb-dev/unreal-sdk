// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Containers/ThirdwebMultipartFormData.h"

#include "Misc/FileHelper.h"
#include "Misc/Guid.h"

void FThirdwebMultipartFormData::AddField(const FString& Name, const FString& Value)
{
	FString Field = TEXT("--") + Boundary + TEXT("\r\n");
	Field += TEXT("Content-Disposition: form-data; name=\"") + Name + TEXT("\"\r\n\r\n");
	Field += Value + TEXT("\r\n");

	AddContent(Field);
}

void FThirdwebMultipartFormData::AddFile(const FString& FieldName, const FString& Filename, const TArray<uint8>& FileContent)
{
	FString Header = TEXT("--") + Boundary + TEXT("\r\n");
	Header += TEXT("Content-Disposition: form-data; name=\"") + FieldName + TEXT("\"; filename=\"") + Filename + TEXT("\"\r\n");
	Header += TEXT("Content-Type: application/octet-stream\r\n\r\n");

	AddContent(Header);
	FormData.Append(FileContent);
	AddContent(TEXT("\r\n"));
}

TArray<uint8> FThirdwebMultipartFormData::GetContent()
{
	FString Footer = TEXT("--") + Boundary + TEXT("--\r\n");
	AddContent(Footer);
	return FormData;
}

FString FThirdwebMultipartFormData::GenerateBoundary()
{
	return TEXT("----WebKitFormBoundary") + FGuid::NewGuid().ToString();
}

void FThirdwebMultipartFormData::AddContent(const FString& Content)
{
	FTCHARToUTF8 Converted(*Content);
	FormData.Append(reinterpret_cast<const uint8*>(Converted.Get()), Converted.Length());
}

