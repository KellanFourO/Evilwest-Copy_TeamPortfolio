#include "stdafx.h"
#include "Window_UITool.h"
#include "UI_Anything.h"
#include "GameInstance.h"

#include "Json_Utility.h"

/* error �ܺ����� ��ȣ : define �ɾ������ */
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

CWindow_UITool::CWindow_UITool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CImgui_Window(pDevice, pContext)
{
}


HRESULT CWindow_UITool::Initialize()
{
	//! ����� Ư���� ��ɾ���. ���� �ʿ��� �� ���Ƽ� ����.
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	/* �ش� ��ξȿ� �ִ� ��� �̹������� �ҷ��´�. */
	LoadImg(ConverCtoWC(ConverWStringtoC(TEXT("../Bin/Resources/Textures/UI/Textures/PlayerHUD"))));
	//LoadImg(ConverCtoWC(ConverWStringtoC(TEXT("../Bin/Resources/Textures/UI/Textures"))));

	// �̹��� �ε� Test
	_int iSize = m_vecPaths.size();
	for (_int i = 0; i < iSize; i++)
	{
		IMAGEINFO* tTexture = new IMAGEINFO;

		_bool bRet = LoadTextureFromFile(ConverWStringtoC(ConvertToWideString(m_vecPaths[i]->strFilePath)), &tTexture->SRV_Texture, &tTexture->iImage_Width, &tTexture->iImage_Height);
		IM_ASSERT(bRet);

		/* Texture ũ�⸦ ���Ƿ� �����ϰ� �ʹٸ�, ���⼭ ������ �������. ���� ���ָ� ���� �ؽ�ó ũ���� ���´�. �� �� ���� ũ�⸦ �̿��� ������ �ٿ��� ����ص� ���� �� ����. */
		//tTexture->iImage_Width = 100;
		//tTexture->iImage_Height = 100;
		tTexture->iTextureNum = i;
		m_vecTexture.push_back(tTexture);

	}

	for (auto& iter : m_vecPaths)
	{
		string strFileNameWithoutExtension = GetFileName(iter->strFilePath);
		string strFileName = RemoveExtension(strFileNameWithoutExtension);

		/* ��� �߶� �ֱ� */
		m_vecImagePaths.push_back(ConverWStringtoC(ConvertToWideString(strFileName)));
	}

#pragma region �������
	//char filePath[MAX_PATH] = "../Bin/DataFiles/Data_UI/Texture_Info/Texture_Info";

	//json Out_Json;
	//_ushort iIndex = 0;

	//for (PATHINFO* iter : m_vecPaths)
	//{
	//	json object;
	//	object["PathNum"] = iter->iPathNum;
	//	object["FileName"] = iter->strFileName;
	//	object["FilePath"] = iter->strFilePath;
	//	Out_Json.emplace(to_string(iIndex++), object);
	//}

	//CJson_Utility::Save_Json(filePath, Out_Json);

	//CJson_Utility::Load_Json(filePath, Out_Json);
#pragma endregion 

	return S_OK;
}

void CWindow_UITool::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	__super::Begin();
	ImGui::Text("UI_Tool");

	/* List */
	UI_List(fTimeDelta);

	/* UI_2D ���� */
	UI2D_Setting(fTimeDelta);

	/* �̹��� ���� �� �̸����� */
	ImagePreview(fTimeDelta);

	ImGui::Dummy(ImVec2(0, 5.f)); // ����

	/* Default : LastNumber */
	UI_ToolTip(fTimeDelta); // Tip : ������ �������̽� ��� ����� ���� �������� ȣ��Ǿ���Ѵ�. (�ȱ׷��� �ٸ� �༮���� ������ �з��� ��µ�)
	__super::End();
}

void CWindow_UITool::Render()
{

}

void CWindow_UITool::UI_List(_float fTimeDelta)
{
	//if (m_vecUIObject.empty())
	//	return;

	static int	Object_idx = 0;
	static int	Texture_idx = 0;
	static int	Layer_idx = 0;

	if (ImGui::BeginListBox("LayerList"))
	{
		for (_int i = 0; i < 3; i++)
		{
			const bool is_selected = (Layer_idx == i);
			if (ImGui::Selectable(m_strItems[i].c_str(), is_selected))
				Layer_idx = i;
			m_iLayerNum = i;
			
			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndListBox();
	}

	ImGui::Spacing();

	_int		iTextureTagSize = (_int)m_vecImagePaths.size();
	if (ImGui::BeginListBox("TextureList"))
	{
		for (_int i = 0; i < iTextureTagSize; i++)
		{
			const bool is_selected = (m_iSelectedPathIndex == i);
			if (ImGui::Selectable(m_vecImagePaths[i], is_selected))
			{			
				m_iSelectedPathIndex = i;
			}
			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	//ImGui::Text("pointer = %p", m_vecTexture[m_iSelectedPathIndex]->SRV_Texture);
	//ImGui::Text("size = %f x %f", m_vecTexture[m_iSelectedPathIndex]->iImage_Width, m_vecTexture[m_iSelectedPathIndex]->iImage_Height);
	ImGui::Image((void*)m_vecTexture[m_iSelectedPathIndex]->SRV_Texture, ImVec2(m_vecTexture[m_iSelectedPathIndex]->iImage_Width, m_vecTexture[m_iSelectedPathIndex]->iImage_Height));

	/* �ױ� */
	m_tUI_Desc.strProtoTag = m_vecImagePaths[m_iSelectedPathIndex];

	ImGui::Spacing();
	_int		ObjectTagSize = (_int)m_vecObjectName.size();
	if (ImGui::BeginListBox("ObjectList"))
	{
		for (_int i = 0; i < ObjectTagSize; i++)
		{
			const bool is_selected = (Object_idx == i);
			if (ImGui::Selectable(ConverWStringtoC(ConvertToWideString(m_vecObjectName[i])), is_selected))
			{
				Object_idx = i;
			}
			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	if (!m_vecUIObject.empty())
	{
		_vector vPosition = m_vecUIObject[Object_idx]->Get_Transform()->Get_State(CTransform::STATE_POSITION);

		ImGui::InputFloat("PositionX", &vPosition.m128_f32[0]);
		ImGui::InputFloat("PositionY", &vPosition.m128_f32[1]);
		ImGui::InputFloat("PositionZ", &vPosition.m128_f32[2]);
	}


	///* Test Value */
	//m_tUI_Info.strName = "Test UI List";
	//m_tUI_Info.iNum = 1;
	//m_tUI_Info.fNum = 1.5f;

	//// ������ ������� ǥ��
	//if (ImGui::TreeNode(m_tUI_Info.strName.c_str())) {
	//	ImGui::Text(u8"��1 : %d", m_tUI_Info.iNum);
	//	ImGui::Text(u8"��2 : %.2f", m_tUI_Info.fNum);

	//	// ��ư
	//	if (ImGui::Button(u8"��ư"))
	//	{

	//	}

	//	ImGui::TreePop();
	//}
}

void CWindow_UITool::UI_ToolTip(_float fTimeDelta)
{
	/* �׽�Ʈ �� */
	_int	iTestNum = 1;
	_float	fTestValue1 = 33.f;
	_float	fTestValue2 = 44.f;

	/* ���콺 ������ ��ġ */
	_float	fTestX = 50.f;
	_float	fTestY = 500.f;

	/* ���콺 ������ ���� */
	_float	fHoverRangeX = 200.f;
	_float	fHoverRangeY = 200.f;

	/* ���콺 ������ ������ ����â�� ũ�� */
	_float	fScreenPosX = 20.f;
	_float	fScreenPosY = 40.f;

	//// ���콺�� �ش� ��ġ ���� �ִ� ��쿡�� ���� ǥ�� (���콺 ����)
	//if (ImGui::IsMouseHoveringRect(
	//	ImVec2(fTestX - fHoverRangeX, fTestY - fHoverRangeY),
	//	ImVec2(fTestX + fHoverRangeX, fTestY + fHoverRangeY)))
	//{

	//	// ǥ���� ����
	//	ImGui::SetCursorScreenPos(ImVec2(fTestX + fScreenPosX, fTestY - fScreenPosY));
	//	ImGui::BeginTooltip();

	//	ImGui::EndTooltip(); // ���� ����
	//}
}

// �̹����� �޸𸮿� �ε��ϴ� �Լ�
std::vector<unsigned char> CWindow_UITool::UI_LoadImage(const std::string& filename, int& width, int& height, int& channels)
{
	// ���� ���� �б� (�̸������ �ִ� 100����Ʈ������ ���)
	std::ifstream file(filename, std::ios::binary);
	if (file.is_open()) {
		char buffer[101]; // 100����Ʈ + �� ����
		if (file.read(buffer, 100)) {
			buffer[file.gcount()] = '\0'; // �� ���� �߰�
			std::cout << "Preview: " << buffer << std::endl;
		}
		file.close();
	}
	else {
		std::cerr << "Failed to open file: " << filename << std::endl;
	}

	// ���� ũ�� Ȯ��
	file.seekg(0, std::ios::end);
	std::streamsize fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	// ���� ������ ������ ���� ����
	std::vector<unsigned char> imageData(fileSize);
	if (!file.read(reinterpret_cast<char*>(imageData.data()), fileSize)) {
		std::cerr << "Failed to read file: " << filename << std::endl;
		return {};
	}

	// �̹��� �ʺ�, ����, ä�� �� ���� (���÷� ���� �� ���)
	width = 256;
	height = 256;
	channels = 4; // RGBA

	return imageData;
}

// Simple helper function to load an image into a DX11 texture with common settings
bool CWindow_UITool::LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	m_pDevice->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	m_pDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();

	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);

	return true;
}

// std::string ���͸� const char* �迭�� ��ȯ�ϴ� �Լ�
std::vector<const char*> CWindow_UITool::ConvertStringVectorToCharArray(const std::vector<std::string>& stringVector)
{
	std::vector<const char*> charArray;
	charArray.reserve(stringVector.size());
	for (const std::string& str : stringVector)
	{
		charArray.push_back(str.c_str());
	}
	return charArray;
}

std::wstring CWindow_UITool::ConvertToWideString(const std::string& ansiString)
{
	int wideStrLen = MultiByteToWideChar(CP_ACP, 0, ansiString.c_str(), -1, nullptr, 0);
	if (wideStrLen == 0)
	{
		// ��ȯ ���� ó��
		return L"Fail";
	}

	std::wstring wideString(wideStrLen, L'\0');
	MultiByteToWideChar(CP_ACP, 0, ansiString.c_str(), -1, &wideString[0], wideStrLen);

	return wideString;
}

std::string CWindow_UITool::WStringToString(const std::wstring& wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	return converter.to_bytes(wstr);
}

char* CWindow_UITool::ConverWStringtoC(const wstring& wstr)
{
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	char* result = new char[size_needed];
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, result, size_needed, NULL, NULL);
	return result;
}

wchar_t* CWindow_UITool::ConverCtoWC(char* str)
{
	_tchar* pStr;
	int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);
	pStr = new WCHAR[strSize];
	MultiByteToWideChar(CP_ACP, 0, str, (_int)strlen(str) + (size_t)1, pStr, strSize);

	return pStr;
}

std::string CWindow_UITool::WideStringToString(const wchar_t* wideStr) 
{
	// std::wstring���κ��� std::string���� ��ȯ
	std::wstring wstr(wideStr);
	// std::string���� ��ȯ
	return std::string(wstr.begin(), wstr.end());
}

// ���� �̸��� �����ϴ� �Լ�
std::string CWindow_UITool::GetFileName(const std::string& filePath) 
{
	size_t lastSlashPos = filePath.find_last_of("/");
	if (lastSlashPos != std::string::npos) 
	{
		return filePath.substr(lastSlashPos + 1);
	}
	else 
	{
		// ��� �����ڰ� ���� ��� ��ü ��θ� ��ȯ
		return filePath;
	}
}

std::string CWindow_UITool::RemoveExtension(const std::string& filePath)
{
	size_t lastDotPos = filePath.find_last_of(".");
	if (lastDotPos != std::string::npos) 
	{
		return filePath.substr(0, lastDotPos);
	}
	else 
	{
		// Ȯ���ڰ� ���� ��� �״�� ��ȯ
		return filePath;
	}
}

WCHAR* CWindow_UITool::StringTowchar(const std::string& str)
{
	// std::wstring���� ��ȯ
	std::wstring wstr(str.begin(), str.end());
	// c_str() �Լ��� ����Ͽ� WCHAR* �����ͷ� ��ȯ
	return const_cast<WCHAR*>(wstr.c_str());
}

void CWindow_UITool::LoadImg(const _tchar* folderPath)
{
	// ã�� �̹��� �����͸� ���� ����
	WIN32_FIND_DATA findData;

	// ���� ���
	wstring wfolderPath = (wstring)folderPath + L"/*.*";

	// ���� ��� ���� �� �ڵ� ��ȯ
	HANDLE hFind = FindFirstFile(wfolderPath.c_str(), &findData);

	// �ڵ��� ���� ���
	if (hFind != INVALID_HANDLE_VALUE)
	{
		// ���� ���丮
		vector<wstring> subDirectories;
		// �̹��� ����
		vector<wstring> imageFiles;
		do
		{
			//  ������ �Ӽ� ������ �Էµȴ�. (���丮���� �������� ��)
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (lstrcmp(findData.cFileName, L".") != 0 && lstrcmp(findData.cFileName, L"..") != 0)
				{
					// ���� ��� = ��� + / + ���ϸ�
					wstring subFolderPath = (wstring)folderPath + L"/" + findData.cFileName;
					subDirectories.push_back(subFolderPath);
					LoadImg(subFolderPath.c_str());
				}
			}
			else
			{
				// ������ ���, �̹��� �������� Ȯ���ϰ� �ε�
				wstring filePath = (wstring)folderPath + L"/" + findData.cFileName;

				// ���� Ȯ���� Ȯ��
				if (wcsstr(findData.cFileName, L".png") || wcsstr(findData.cFileName, L".jpg") ||
					/*wcsstr(findData.cFileName, L".bmp") || wcsstr(findData.cFileName, L".tga") ||*/
					wcsstr(findData.cFileName, L".dds"))
				{
					PATHINFO* pPathInfo = new PATHINFO;

					pPathInfo->strFilePath = WStringToString(filePath);
					//wcscpy(&pPathInfo->cFileName[MAX_PATH], StringTowchar(RemoveExtension(WideStringToString(findData.cFileName))));
					pPathInfo->strFileName = RemoveExtension(WStringToString(findData.cFileName));
					//pPathInfo->cFileName[MAX_PATH] = *StringTowchar(RemoveExtension(WideStringToString(findData.cFileName)));
					pPathInfo->iPathNum = m_iTextureNum;
					m_vecPaths.push_back(pPathInfo);

					m_iTextureNum++;
				}
			}
		} while (FindNextFile(hFind, &findData));

		
		FindClose(hFind);
	}
}

void CWindow_UITool::ImagePreview(_float fTimeDelta)
{

}

void CWindow_UITool::UI2D_Setting(_float fTimeDelta)
{
	ImGui::CollapsingHeader("2D_Setting");
	/* Scale */
	ImGui::SeparatorText(u8"ũ�� ����");
	ImGui::InputFloat("ScaleX", &m_tUI_Desc.fSizeX);
	ImGui::InputFloat("ScaleY", &m_tUI_Desc.fSizeY);

	/* Position*/
	ImGui::SeparatorText(u8"��ġ ����");
	ImGui::InputFloat("PositionX", &m_tUI_Desc.fX);
	ImGui::InputFloat("PositionY", &m_tUI_Desc.fY);

	ImGui::Dummy(ImVec2(0, 2.5)); // ����

	ImGui::Separator();

#pragma region Create/Delete
	if (ImGui::Button("Create"))
	{
		UI2D_Create(fTimeDelta);
	}

	ImGui::SameLine(70.f);

	if (ImGui::Button("Delete"))
	{
		UI2D_Delete(fTimeDelta);
	}
#pragma endregion End

#pragma region Save/Load
	if (ImGui::Button("Save"))
	{	
		Save_Desc();
	}

	ImGui::SameLine(70.f);

	if (ImGui::Button("Load"))
	{
		Load_Desc();
	}
#pragma endregion End
}

HRESULT CWindow_UITool::UI2D_Create(_float fTimeDelta)
{
	FAILED_CHECK(m_pGameInstance->Add_CloneObject(LEVEL_STATIC, ConvertToWideString(m_strItems[m_iLayerNum]), TEXT("Prototype_GameObject_UI_Anything"), &m_tUI_Desc));

	m_vecObjectName.push_back(m_tUI_Desc.strProtoTag);
	m_pGameInstance->Get_CloneGameObjects(LEVEL_STATIC, &m_vecUIObject);
	m_iUINameNum++;
}

void CWindow_UITool::UI2D_Delete(_float fTimeDelta)
{

}

/* ex : Save */
void CWindow_UITool::Save_Desc()
{
	//string filePath = "Particle_Desc";
	char filePath[MAX_PATH] = "../Bin/DataFiles/Data_UI/UI_Info";

	json Out_Json;
	
	Out_Json["PostionX"] = m_tUI_Info.fX;
	Out_Json["PostionY"] = m_tUI_Info.fY;
	Out_Json["SizeX"] = m_tUI_Info.fSizeX;
	Out_Json["SizeY"] = m_tUI_Info.fSizeY;

	CJson_Utility::Save_Json(filePath, Out_Json);
}

/* ex : Load */
void CWindow_UITool::Load_Desc()
{
	json json_in;
	char filePath[MAX_PATH] = "../Bin/DataFiles/Data_UI/UI_Info";

	CJson_Utility::Load_Json(filePath, json_in);

	m_tUI_Info.fX = json_in["PostionX"];
	m_tUI_Info.fY = json_in["PostionY"];
	m_tUI_Info.fSizeX = json_in["SizeX"];
	m_tUI_Info.fSizeY = json_in["SizeY"];

}

// ImGui�� ����Ͽ� �̹����� ǥ���ϴ� �Լ�
void CWindow_UITool::ShowImagePreview(const std::vector<unsigned char>& imageData, int width, int height)
{
	ImVec2 imageSize(width, height);
	ImGui::Image(reinterpret_cast<void*>(const_cast<unsigned char*>(imageData.data())), imageSize);
}

CWindow_UITool* CWindow_UITool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWindow_UITool* pInstance = new CWindow_UITool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CWindow_UITool");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWindow_UITool::Free()
{
	__super::Free();
}