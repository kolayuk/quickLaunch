/*
========================================================================
 Name        : MainView.cpp
 Author      : Usanov-Kornilov Nikolay (aka Kolay)
 Copyright   : 
Contacts:
	kolayuk@mail.ru
	http://kolaysoft.ru

(c) KolaySoft, 2011

 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated System Includes]
#include <aknviewappui.h>
#include <eikmenub.h>
#include <avkon.hrh>
#include <barsread.h>
#include <stringloader.h>
#include <aknlists.h>
#include <eikenv.h>
#include <akniconarray.h>
#include <eikclbd.h>
#include <akncontext.h>
#include <akntitle.h>
#include <eikbtgpc.h>
#include <quickLauncher.rsg>
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated User Includes]

#include "quickLauncher.hrh"
#include "MainView.h"
#include "Main.hrh"
#include "Main.h"
// ]]] end generated region [Generated User Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

#include <quickLauncherAppUi.h>
#include <aknpopup.h>
#include <apgcli.h>
#include <aknselectionlist.h>
#include <aknnotedialog.h>
#include <aknnotewrappers.h>
#include <OVIUtils.h>
#include <logger.h>

struct TApplicationInfo
{
	TFileName iCaption;
	TUid iUid;
	static TInt Compare(const TApplicationInfo& aInfo1, const TApplicationInfo& aInfo2)
	{
		return aInfo1.iCaption.CompareC(aInfo2.iCaption);
	}
};


CMainView::CMainView()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	iMain = NULL;
	// ]]] end generated region [Generated Contents]
	
	}

/** 
 * The view's destructor removes the container from the control
 * stack and destroys it.
 */
CMainView::~CMainView()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	delete iMain;
	iMain = NULL;
	// ]]] end generated region [Generated Contents]
	
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance then calls the second-phase constructor
 * without leaving the instance on the cleanup stack.
 * @return new instance of CMainView
 */
CMainView* CMainView::NewL()
	{
	CMainView* self = CMainView::NewLC();
	CleanupStack::Pop( self );
	return self;
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance, pushes it on the cleanup stack,
 * then calls the second-phase constructor.
 * @return new instance of CMainView
 */
CMainView* CMainView::NewLC()
	{
	CMainView* self = new ( ELeave ) CMainView();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}


/**
 * Second-phase constructor for view.  
 * Initialize contents from resource.
 */ 
void CMainView::ConstructL()
	{
	// [[[ begin generated region: do not modify [Generated Code]
	BaseConstructL( R_MAIN_MAIN_VIEW );
				
	// ]]] end generated region [Generated Code]
	
	// add your own initialization code here
	
	}

/**
 * @return The UID for this view
 */
TUid CMainView::Id() const
	{
	return TUid::Uid( EMainViewId );
	}

/**
 * Handle a command for this view (override)
 * @param aCommand command id to be handled
 */
void CMainView::HandleCommandL( TInt aCommand )
	{
	// [[[ begin generated region: do not modify [Generated Code]
	TBool commandHandled = EFalse;
	switch ( aCommand )
		{	// code to dispatch to the AknView's menu and CBA commands is generated here
	
		case EAknSoftkeyExit:
			commandHandled = Hide( aCommand );
			break;
		case EMainView_MenuItem1Command:
			commandHandled = About( aCommand );
			break;
		case EMainView_MenuItem2Command:
			commandHandled = Other( aCommand );
			break;
		case EMainView_MenuItem3Command:
			commandHandled = Exit( aCommand );
			break;
		case EMainViewExceptions:
			SelectExceptions();
			break;
		default:
			break;
		}
	
		
	if ( !commandHandled ) 
		{
	
		if ( aCommand == EMainViewControlPaneRightId )
			{
			AppUi()->HandleCommandL( EEikCmdExit );
			}
	
		}
	// ]]] end generated region [Generated Code]
	
	}

/**
 *	Handles user actions during activation of the view, 
 *	such as initializing the content.
 */
void CMainView::DoActivateL( 
		const TVwsViewId& /*aPrevViewId*/,
		TUid /*aCustomMessageId*/,
		const TDesC8& /*aCustomMessage*/ )
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	SetupStatusPaneL();
	
				
				
	
	if ( iMain == NULL )
		{
		iMain = CreateContainerL();
		iMain->SetMopParent( this );
		AppUi()->AddToStackL( *this, iMain );
		} 
	// ]]] end generated region [Generated Contents]
	InitValues();
	}

/**
 */
void CMainView::DoDeactivate()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	CleanupStatusPane();
	
	if ( iMain != NULL )
		{
		AppUi()->RemoveFromViewStack( *this, iMain );
		delete iMain;
		iMain = NULL;
		}
	// ]]] end generated region [Generated Contents]
	
	}

/** 
 * Handle status pane size change for this view (override)
 */
void CMainView::HandleStatusPaneSizeChange()
	{
	CAknView::HandleStatusPaneSizeChange();
	
	// this may fail, but we're not able to propagate exceptions here
	TVwsViewId view;
	AppUi()->GetActiveViewId( view );
	if ( view.iViewUid == Id() )
		{
		TInt result;
		TRAP( result, SetupStatusPaneL() );
		}
	
	// [[[ begin generated region: do not modify [Generated Code]
	// ]]] end generated region [Generated Code]
	
	}

// [[[ begin generated function: do not modify
void CMainView::SetupStatusPaneL()
	{
	// reset the context pane
	TUid contextPaneUid = TUid::Uid( EEikStatusPaneUidContext );
	CEikStatusPaneBase::TPaneCapabilities subPaneContext = 
		StatusPane()->PaneCapabilities( contextPaneUid );
	if ( subPaneContext.IsPresent() && subPaneContext.IsAppOwned() )
		{
		CAknContextPane* context = static_cast< CAknContextPane* > ( 
			StatusPane()->ControlL( contextPaneUid ) );
		context->SetPictureToDefaultL();
		}
	
	// setup the title pane
	TUid titlePaneUid = TUid::Uid( EEikStatusPaneUidTitle );
	CEikStatusPaneBase::TPaneCapabilities subPaneTitle = 
		StatusPane()->PaneCapabilities( titlePaneUid );
	if ( subPaneTitle.IsPresent() && subPaneTitle.IsAppOwned() )
		{
		CAknTitlePane* title = static_cast< CAknTitlePane* >( 
			StatusPane()->ControlL( titlePaneUid ) );
		TResourceReader reader;
		iEikonEnv->CreateResourceReaderLC( reader, R_MAIN_TITLE_RESOURCE );
		title->SetFromResourceL( reader );
		CleanupStack::PopAndDestroy(); // reader internal state
		}
				
	}

// ]]] end generated function

// [[[ begin generated function: do not modify
void CMainView::CleanupStatusPane()
	{
	}

// ]]] end generated function

CMain* CMainView::CreateContainerL()
	{
	return CMain::NewL( ClientRect(), NULL, this );
	}

TBool CMainView::Hide( TInt aCommand )
	{
	if (((CquickLauncherAppUi*)AppUi())->iSettings[0]==1)
		{CEikonEnv::Static()->RootWin().SetOrdinalPosition(-4);}
	else
		{AppUi()->Exit();}
	return ETrue;
	}
				
TBool CMainView::Edit( TInt aCommand )
	{
	if (iMain->ListBox()->Model()->NumberOfItems()==0){return ETrue;}
	HandleLBSelect(iMain->ListBox()->CurrentItemIndex());
	return ETrue;
	}
				
TBool CMainView::About( TInt aCommand )
	{
	OVIUtils::AboutDialog();
	return ETrue;
	}
				
TBool CMainView::Other( TInt aCommand )
	{
	OVIUtils::GoToOvi();
	return ETrue;
	}
				
TBool CMainView::Exit( TInt aCommand )
	{
	Hide(1);
	return ETrue;
	}
void CMainView::HandleLBSelect(TInt aIndex)
	{
	TBuf<255> title;
	title.Copy(GetTitle(aIndex));
	TUint32 val;
	if (aIndex<((CquickLauncherAppUi*)AppUi())->iSettings.Count())
		{val=((CquickLauncherAppUi*)AppUi())->iSettings[aIndex];}
	TUint32 new_val;
	TBuf<255> txt;
	if (aIndex==8)
			{SelectExceptions();return;}
	if (aIndex==0) // app work
		{
		if (val==0){new_val=1;}
		else {new_val=0;}
		CDesC16Array* arr=CEikonEnv::Static()->ReadDesC16ArrayResourceL(R_YES_NO);
		txt.Copy(arr->MdcaPoint(new_val));
		delete arr;
		}
	else if (aIndex==1) // autostart
		{
		if (val==0){new_val=1;}
		else {new_val=0;}
		CDesC16Array* arr=CEikonEnv::Static()->ReadDesC16ArrayResourceL(R_YES_NO);
		txt.Copy(arr->MdcaPoint(new_val));
		delete arr;
		}
	else if (aIndex==2) // position (left\right)
		{
		CDesC16Array* arr=CEikonEnv::Static()->ReadDesC16ArrayResourceL(R_LEFT_RIGHT);
		new_val=PopupMenu(title,arr);
		if (new_val==-1){return;}
		txt.Copy(arr->MdcaPoint(new_val));
		delete arr;
		}
	else if (aIndex==3||aIndex==4||aIndex==5||aIndex==6||aIndex==7) // apps
		{
		TInt32 sel=ApplicationDialog();
		_LOGDATA(_L("AppSelected: %d"),sel);
		if (sel==-1){return;}
		if ((((CquickLauncherAppUi*)AppUi())->iSettings.Find(sel)!=KErrNotFound)&&(sel!=0))
			{
			_LOG(_L("App already added"));
			TBuf<255> warn;
			CEikonEnv::Static()->ReadResource(warn,R_APP_EXISTS);
			CAknErrorNote* dialog = new(ELeave)CAknErrorNote();
			dialog->ExecuteLD(warn);
			return;
			}
		new_val=sel;
		RApaLsSession ls;
		ls.Connect();
		TApaAppInfo info;
		_LOG(_L("Will get info"));
		ls.GetAppInfo(info,TUid::Uid(sel));
		_LOGDATA(_L("Info getted %S"),&info.iCaption);
		ls.Close();
		txt.Copy(info.iCaption);
		if (sel==0){CEikonEnv::Static()->ReadResource(txt,R_APP_NOT_SELECTED);}
		}
	_LOG(_L("Will update settings"));
	((CquickLauncherAppUi*)AppUi())->iSettings[aIndex]=new_val;
	((CquickLauncherAppUi*)AppUi())->UpdateSettings();
	ChangeItemSubtitle(aIndex,txt);
	((CquickLauncherAppUi*)AppUi())->WriteSettings();
	}
TPtrC CMainView::GetTitle(TInt aIndex)
	{
	TBuf<255> prev = iMain->ListBox()->Model()->ItemText(aIndex);
	TChar a = '\t';
	prev.Copy(prev.Right(prev.Length()-1));
	TInt pos = prev.Find(_L("\t"));
	return prev.Left(pos);
	}
void CMainView::ChangeItemSubtitle(TInt aIndex,TDes16& aText)
	{
	TBuf<512> buf;
	TBuf<255> t; t.Copy(GetTitle(aIndex));
	iMain->CreateListBoxItemL(buf,t,aText);
	CDesCArray* itemArray = static_cast<CDesCArray*> (iMain->ListBox()->Model()->ItemTextArray());
	itemArray->Delete(aIndex);
	itemArray->InsertL(aIndex,buf);
	iMain->ListBox()->DrawNow();
	}

TInt CMainView::PopupMenu(TDes& aTxt,CDesCArray* itemList)
	{
	_LIT(KListItemFormat, "%S");
	CAknSinglePopupMenuStyleListBox* list = new(ELeave) CAknSinglePopupMenuStyleListBox;
	CleanupStack::PushL(list);
	CAknPopupList* popupList = CAknPopupList::NewL(list, R_AVKON_SOFTKEYS_OK_BACK, AknPopupLayouts::EMenuWindow);
	CleanupStack::PushL(popupList);    
	list->ConstructL(popupList, CEikListBox::ELeftDownInViewRect);
	list->CreateScrollBarFrameL(ETrue);
	list->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);
		
	list->Model()->SetItemTextArray(itemList);
	list->Model()->SetOwnershipType(ELbmOwnsItemArray);
	list->ItemDrawer()->FormattedCellData()->EnableMarqueeL( ETrue );
	popupList->SetTitleL(aTxt);    
	CleanupStack::Pop();            // popuplist
	CleanupStack::Pop();	//list
	TBool popupOk = popupList->ExecuteLD();
	if (!popupOk)
		{
		return -1;
		}
	else 
		{
		TInt current=list->CurrentItemIndex();
		return current;
		}
	}
void CMainView::InitValues()
	{
	RArray<TUint32> arr=((CquickLauncherAppUi*)AppUi())->iSettings;
	TBuf <255> txt; 
	for (TInt aIndex=0;aIndex<arr.Count();aIndex++)
		{
		if (aIndex==0) // app work
			{
			CDesC16Array* arr2=CEikonEnv::Static()->ReadDesC16ArrayResourceL(R_YES_NO);
			txt.Copy(arr2->MdcaPoint(arr[aIndex]));
			delete arr2;
			}
		else if (aIndex==1) // autostart
			{
			CDesC16Array* arr2=CEikonEnv::Static()->ReadDesC16ArrayResourceL(R_YES_NO);
			txt.Copy(arr2->MdcaPoint((TInt)arr[aIndex]));
			delete arr2;
			}
		else if (aIndex==2) // position (left\right)
			{
			CDesC16Array* arr2=CEikonEnv::Static()->ReadDesC16ArrayResourceL(R_LEFT_RIGHT);
			txt.Copy(arr2->MdcaPoint((TInt)arr[aIndex]));
			delete arr2;
			}
		else if (aIndex==3||aIndex==4||aIndex==5||aIndex==6||aIndex==7)
			{
			if (arr[aIndex]==0)
				{CEikonEnv::Static()->ReadResource(txt,R_APP_NOT_SELECTED);}
			else
				{
				RApaLsSession ls;
				ls.Connect();
				TApaAppInfo info;
				ls.GetAppInfo(info,TUid::Uid(arr[aIndex]));
				ls.Close();
				txt.Copy(info.iCaption);
				}
			}
		ChangeItemSubtitle(aIndex,txt);
		}
	}


TInt32 CMainView::ApplicationDialog()
	{
	TInt sel=0;
		TInt def=0;
		TBuf<255> result;
		CDesCArray* itemList = new (ELeave) CDesCArrayFlat(3);
		TInt i=0;
		RApaLsSession AppSession;
		AppSession.Connect();
		RArray<TApplicationInfo> apps;
		TApaAppInfo appInfo;
		AppSession.GetAllApps();
		TBuf<255> UidTxt;
		while (AppSession.GetNextApp(appInfo)==KErrNone)
			{
			HBufC* fn;
			if (AppSession.GetAppIcon(appInfo.iUid,fn)!=KErrNone){continue;}
			if (fn){delete fn;}
			if (appInfo.iCaption.Length()<2){continue;}
			TApplicationInfo info;
			info.iCaption=appInfo.iCaption;
			info.iUid=appInfo.iUid;
			apps.Append(info);
			}
		AppSession.Close();
		TLinearOrder<TApplicationInfo> sortOrder(TApplicationInfo::Compare); 
		apps.Sort(sortOrder);
		for (TInt i=0;i<apps.Count();i++)
			{
			TBuf<255> item;
			item.Copy(_L("\t"));
			item.Append(apps[i].iCaption);
			item.Append(_L(" ["));
			UidTxt.Num((TUint)apps[i].iUid.iUid,EHex);
			UidTxt.UpperCase();
			item.Append(UidTxt);
			item.Append(_L("]"));
			item.Append(_L("\t"));
			itemList->AppendL(item);
			}
		TBuf<255> emptyres,emptytxt;
		emptytxt.Copy(_L("\t"));
		CEikonEnv::Static()->ReadResource(emptyres,R_APP_NOT_SELECTED);
		emptytxt.Append(emptyres);
		emptytxt.Append(_L("\t"));
		itemList->InsertL(0,emptytxt);
		TApplicationInfo empty;
		empty.iUid=TUid::Uid(0);
		apps.Insert(empty,0);
		CAknSelectionListDialog* dlg=CAknSelectionListDialog::NewL(sel,itemList,R_AVKON_DIALOG_EMPTY_MENUBAR);
		dlg->PrepareLC(R_APP_SELECTOR);
		((CAknSingleStyleListBox*)dlg->Control(ESelectionListControl))->ItemDrawer()->ColumnData()->EnableMarqueeL(ETrue);
		//((CEikListBox*)dlg->Control(ESelectionListControl))->SetCurrentItemIndex(def);
		TInt popupOk=dlg->RunLD();

		if (!popupOk)
			{
			delete itemList;
			apps.Close();
			return -1;
			}
		else 
			{
			TUint32 Uid=apps[sel].iUid.iUid;
			delete itemList;
			apps.Close();
			return Uid;
			}
	}
void CMainView::SelectExceptions()
	{
	TInt sel=0;
	TInt def=0;
	TBuf<255> result;
	CDesCArray* itemList = new (ELeave) CDesCArrayFlat(3);
	CArrayFixFlat<TInt>* DefaultSelectedItemsList=new (ELeave) CArrayFixFlat<TInt>(3);
	TInt i=0;
	RApaLsSession AppSession;
	AppSession.Connect();
	RArray<TApplicationInfo> apps;
	TApaAppInfo appInfo;
	AppSession.GetAllApps();
	TBuf<255> UidTxt;
	while (AppSession.GetNextApp(appInfo)==KErrNone)
		{
		if (appInfo.iCaption.Length()<2){continue;}
		TApplicationInfo info;
		info.iCaption=appInfo.iCaption;
		info.iUid=appInfo.iUid;
		apps.Append(info);
		}
	AppSession.Close();
	TLinearOrder<TApplicationInfo> sortOrder(TApplicationInfo::Compare); 
	apps.Sort(sortOrder);
	for (TInt i=0;i<apps.Count();i++)
		{
		TBuf<255> item;
		item.Copy(_L("\t"));
		item.Append(apps[i].iCaption);
		item.Append(_L(" ["));
		UidTxt.Num((TUint)apps[i].iUid.iUid,EHex);
		UidTxt.UpperCase();
		item.Append(UidTxt);
		item.Append(_L("]"));
		item.Append(_L("\t"));
		itemList->AppendL(item);
		if (((CquickLauncherAppUi*)AppUi())->iExceptions.Find(apps[i].iUid)!=KErrNotFound){DefaultSelectedItemsList->AppendL(i);}
		}
	CAknListQueryDialog* dlg = new ( ELeave ) CAknListQueryDialog( DefaultSelectedItemsList);
	dlg->PrepareLC( R_SISXUI_OPTIONS_QUERY );
	dlg->SetItemTextArray( itemList );
	dlg->ListBox()->SetSelectionIndexesL(DefaultSelectedItemsList);    
    dlg->SetOwnershipType( ELbmDoesNotOwnItemArray );
    ((CEikFormattedCellListBox*)(dlg->ListBox()))->ItemDrawer()->FormattedCellData()->EnableMarqueeL( ETrue );
    TInt popupOk = dlg->RunLD();

	if (!popupOk)
		{
		return;
		}
	else 
		{
		i=0;
		((CquickLauncherAppUi*)AppUi())->iExceptions.Reset();
		for (i=0; i<DefaultSelectedItemsList->Count(); i++)
			{
				TUid uid=apps[DefaultSelectedItemsList->At(i)].iUid;
				((CquickLauncherAppUi*)AppUi())->iExceptions.Append(uid);
			}
		((CquickLauncherAppUi*)AppUi())->WriteExceptions();
		}

	}
