// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

#include "stdafx.h"

#include <ifcpp/model/AttributeObject.h>
#include <ifcpp/IFC4/include/IfcCartesianPoint.h>
#include <ifcpp/IFC4/include/IfcGloballyUniqueId.h>
#include <ifcpp/IFC4/include/IfcIdentifier.h>
#include <ifcpp/IFC4/include/IfcLabel.h>
#include <ifcpp/IFC4/include/IfcLengthMeasure.h>
#include <ifcpp/reader/ReaderUtil.h>

#include "STEPWnd.h"
#include "Resource.h"
#include "MainFrame.h"
#include "IfcQueryMFC.h"
#include "BuildingUtils.h"
#include "IfcQueryDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

STEPWnd::STEPWnd()
{
	m_nComboHeight = 0;
}

STEPWnd::~STEPWnd()
{
}

BEGIN_MESSAGE_MAP(STEPWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void STEPWnd::AdjustLayout()
{
	if (GetSafeHwnd () == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndObjectCombo.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top + m_nComboHeight, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(nullptr, rectClient.left, rectClient.top + m_nComboHeight + cyTlb, rectClient.Width(), rectClient.Height() -(m_nComboHeight+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int STEPWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create Properties Combo \n");
		return -1;      // fail to create
	}

	m_wndObjectCombo.AddString(_T("Application"));
	m_wndObjectCombo.AddString(_T("Properties Window"));
	m_wndObjectCombo.SetCurSel(0);

	CRect rectCombo;
	m_wndObjectCombo.GetClientRect (&rectCombo);

	m_nComboHeight = rectCombo.Height();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Is locked */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* Locked */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void STEPWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void STEPWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void STEPWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void STEPWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void STEPWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void STEPWnd::OnProperties1()
{
	// TODO: Add your command handler code here
}

void STEPWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void STEPWnd::OnProperties2()
{
	// TODO: Add your command handler code here
}

void STEPWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}


void STEPWnd::setBuildingModel( shared_ptr<BuildingModel> ifc_model )
{
	m_ifc_model = ifc_model;
}

void STEPWnd::unselectAllEntities()
{
	m_wndPropList.RemoveAll();
	//m_columnTree.DeleteColumn( 1 );
}

void addSubItems( const shared_ptr<BuildingEntity>& entity, CMFCPropertyGridProperty* entityGroup, bool addChildren )
{
	std::vector<std::pair<std::string, shared_ptr<BuildingObject> > > vec_attributes;
	entity->getAttributes( vec_attributes );

	std::vector<std::pair<std::string, shared_ptr<BuildingObject> > > vec_attributes_inverse;
	entity->getAttributesInverse( vec_attributes_inverse );

	for( size_t ii = 0; ii < vec_attributes.size(); ++ii )
	{
		std::wstring attributeName = s2ws( vec_attributes[ii].first );
		shared_ptr<BuildingObject> attributeValue = vec_attributes[ii].second;

		if( !attributeValue )
		{
			continue;
		}

		shared_ptr<IfcGloballyUniqueId> globalID = dynamic_pointer_cast<IfcGloballyUniqueId>(attributeValue);
		if( globalID )
		{
			entityGroup->AddSubItem( new CMFCPropertyGridProperty( attributeName.c_str(), (_variant_t)globalID->m_value.c_str(), _T( "" ) ) );
			continue;
		}

		shared_ptr<IfcIdentifier> identifier = dynamic_pointer_cast<IfcIdentifier>(attributeValue);
		if( identifier )
		{
			entityGroup->AddSubItem( new CMFCPropertyGridProperty( attributeName.c_str(), (_variant_t)identifier->m_value.c_str(), _T( "" ) ) );
			continue;
		}

		shared_ptr<IfcLabel> label = dynamic_pointer_cast<IfcLabel>(attributeValue);
		if( label )
		{
			entityGroup->AddSubItem( new CMFCPropertyGridProperty( attributeName.c_str(), (_variant_t)label->m_value.c_str(), _T( "" ) ) );
			continue;
		}

		shared_ptr<IfcCartesianPoint> cartesianPoint = dynamic_pointer_cast<IfcCartesianPoint>(attributeValue);
		if( cartesianPoint )
		{
			std::stringstream coordinates;

			if( cartesianPoint->m_Coordinates.size() > 1 )
			{
				if( cartesianPoint->m_Coordinates[0] && cartesianPoint->m_Coordinates[1] )
				{
					coordinates << cartesianPoint->m_Coordinates[0]->m_value << "," << cartesianPoint->m_Coordinates[1]->m_value;

					if( cartesianPoint->m_Coordinates.size() > 2 )
					{
						if( cartesianPoint->m_Coordinates[2] )
						{
							coordinates << "," << cartesianPoint->m_Coordinates[2]->m_value;
						}
					}
				}
			}
			std::string coordinatesStr = coordinates.str();
			std::wstring coordinatesWStr = s2ws( coordinatesStr );
			entityGroup->AddSubItem( new CMFCPropertyGridProperty( attributeName.c_str(), (_variant_t)coordinatesWStr.c_str(), _T( "" ) ) );
			continue;
		}
		
		shared_ptr<BuildingEntity> attributeEntity = dynamic_pointer_cast<BuildingEntity>(attributeValue);
		if( attributeEntity )
		{
			std::wstringstream wstr;
			wstr << L"#" << attributeEntity->m_entity_id << "=" << attributeEntity->className();
			std::stringstream STEPline;
			attributeEntity->getStepLine( STEPline );
			std::wstring STEPLineWstr = s2ws( STEPline.str() );

			// TODO: second or combined attribute browser like IfcQuickBrowser

			CMFCPropertyGridProperty* entityProperty = new CMFCPropertyGridProperty( attributeName.c_str() );
			entityProperty->SetDescription( STEPLineWstr.c_str() );

			entityGroup->AddSubItem( entityProperty );

			if( addChildren )
			{
				addSubItems( attributeEntity, entityProperty, false );
			}

			continue;
		}

		shared_ptr<AttributeObjectVector> attributeVector = dynamic_pointer_cast<AttributeObjectVector>(attributeValue);
		if( attributeVector )
		{
			std::vector<shared_ptr<BuildingObject> >& vecSub = attributeVector->m_vec;

			// TODO: add items
			
		}

		std::wstringstream wstr;
		wstr << attributeValue->className();

		std::wstring attributeValueClassName = s2ws( attributeValue->className() );
		entityGroup->AddSubItem( new CMFCPropertyGridProperty( attributeName.c_str(), (_variant_t)attributeValueClassName.c_str(), _T( "" ) ) );
	}
}

void STEPWnd::setEntitySelected( int entity_id )
{
	unselectAllEntities();

	if( !m_ifc_model )
	{
		return;
	}

	const std::map<int, shared_ptr<BuildingEntity> >& map_entities = m_ifc_model->getMapIfcEntities();


	auto it_find = map_entities.find( entity_id );
	if( it_find == map_entities.end() )
	{
		return;
	}
	const shared_ptr<BuildingEntity>& entity = it_find->second;

	std::wstringstream wstr;
	wstr << L"#" << entity->m_entity_id << "=" << entity->className();
	std::stringstream STEPline;
	entity->getStepLine( STEPline );
	std::wstring STEPLineWstr = s2ws( STEPline.str() );

	CMFCPropertyGridProperty* entityGroup = new CMFCPropertyGridProperty( wstr.str().c_str() );
	entityGroup->SetDescription( STEPLineWstr.c_str() );

	addSubItems( entity, entityGroup, true );

	m_wndPropList.AddProperty( entityGroup );
}

void STEPWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
}

void STEPWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void STEPWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void STEPWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	m_wndObjectCombo.SetFont(&m_fntPropList);
}
