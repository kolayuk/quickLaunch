/*
========================================================================
 Name        : quickLauncherAppUi.cpp
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
#include <eikmenub.h>
#include <akncontext.h>
#include <akntitle.h>
#include <quickLauncher.rsg>
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated User Includes]
#include "quickLauncherAppUi.h"
#include "quickLauncher.hrh"
#include "Main.hrh"
#include "MainView.h"
// ]]] end generated region [Generated User Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

#include <bautils.h> 
#include <const.h>
#include <apacmdln.h>
#include <apgwgnam.h>
#include <aknglobalnote.h>

#include <sensrvchannel.h>
#include <sensrvproximitysensor.h>
#include <sensrvchannelfinder.h>
#include <sensrvchannelinfo.h>
#include <aknutils.h>
#include <centralrepository.h>
#include <logger.h>

CquickLauncherAppUi::CquickLauncherAppUi(): MSensrvDataListener(),MAknsSkinChangeObserver(), MFgrCallBack()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	
	}

/** 
 * The appui's destructor removes the container from the control
 * stack and destroys it.
 */
CquickLauncherAppUi::~CquickLauncherAppUi()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	if (iSensrvChannel){delete iSensrvChannel;}
	if (iHideTimer){delete iHideTimer;}
	if (iInactivityTimer){delete iInactivityTimer;}
	if (iTimer){delete iTimer;}
	if (iContainer){delete iContainer;}
	if (iLockMonitor){delete iLockMonitor;}
	iWinGroup->Close();
	iSkinSession.Close();
	delete iKeyMon;
	iWsSession->Close();
	}

// [[[ begin generated function: do not modify
void CquickLauncherAppUi::InitializeContainersL()
	{
	iMainView = CMainView::NewL();
	AddViewL( iMainView );
	SetDefaultViewL( *iMainView );
	}
// ]]] end generated function

/**
 * Handle a command for this appui (override)
 * @param aCommand command id to be handled
 */
void CquickLauncherAppUi::HandleCommandL( TInt aCommand )
	{
	// [[[ begin generated region: do not modify [Generated Code]
	TBool commandHandled = EFalse;
	switch ( aCommand )
		{ // code to dispatch to the AppUi's menu and CBA commands is generated here
		default:
			break;
		}
	
		
	if ( !commandHandled ) 
		{
		if ( aCommand == EAknSoftkeyExit || aCommand == EEikCmdExit )
			{
			Exit();
			}
		}
	// ]]] end generated region [Generated Code]
	
	}

/** 
 * Override of the HandleResourceChangeL virtual function
 */
void CquickLauncherAppUi::HandleResourceChangeL( TInt aType )
	{
	CAknViewAppUi::HandleResourceChangeL( aType );
	// [[[ begin generated region: do not modify [Generated Code]
	// ]]] end generated region [Generated Code]
	
	}
				
/** 
 * Override of the HandleKeyEventL virtual function
 * @return EKeyWasConsumed if event was handled, EKeyWasNotConsumed if not
 * @param aKeyEvent 
 * @param aType 
 */
TKeyResponse CquickLauncherAppUi::HandleKeyEventL(
		const TKeyEvent& aKeyEvent,
		TEventCode aType )
	{
	// The inherited HandleKeyEventL is private and cannot be called
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	
	return EKeyWasNotConsumed;
	}

/** 
 * Override of the HandleViewDeactivation virtual function
 *
 * @param aViewIdToBeDeactivated 
 * @param aNewlyActivatedViewId 
 */
void CquickLauncherAppUi::HandleViewDeactivation( 
		const TVwsViewId& aViewIdToBeDeactivated, 
		const TVwsViewId& aNewlyActivatedViewId )
	{
	CAknViewAppUi::HandleViewDeactivation( 
			aViewIdToBeDeactivated, 
			aNewlyActivatedViewId );
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	
	}

/**
 * @brief Completes the second phase of Symbian object construction. 
 * Put initialization code that could leave here. 
 */ 
void CquickLauncherAppUi::ConstructL()
	{
	if (BaflUtils::FileExists(CEikonEnv::Static()->FsSession(),KLogsFile))
			{CEikonEnv::Static()->FsSession().Delete(KLogsFile);}
	_LOG(_L("CquickLauncherAppUi::ConstructL()"));
	if (BaflUtils::FileExists(CEikonEnv::Static()->FsSession(),KFirstStartFile))
		{
		CEikonEnv::Static()->FsSession().Delete(KFirstStartFile);
		if (BaflUtils::FileExists(CEikonEnv::Static()->FsSession(),KSettingPath)){CEikonEnv::Static()->FsSession().Delete(KSettingPath);}
		if (BaflUtils::FileExists(CEikonEnv::Static()->FsSession(),KExceptionsPath)){CEikonEnv::Static()->FsSession().Delete(KExceptionsPath);}
		}
	iSliderOpened=EFalse;
	iExceptionFocused=EFalse;
	iLocked=EFalse;
	ReadSettings();
	ReadPeriods();
	ReadExceptions();
	// [[[ begin generated region: do not modify [Generated Contents]
	
	BaseConstructL( EAknEnableSkin  | 
					 EAknEnableMSK ); 
	InitializeContainersL();
	// ]]] end generated region [Generated Contents]
	iWsSession=new (ELeave)RWsSession();
	iWsSession->Connect();
	
	iWinGroup=new (ELeave) RWindowGroup(CEikonEnv::Static()->WsSession());
	iWinGroup->Construct((TUint32)&iWinGroup, EFalse);
	iWinGroup->EnableReceiptOfFocus(EFalse); // Don't capture any key events.
	iWinGroup->SetOrdinalPosition(0, ECoeWinPriorityAlwaysAtFront);

	CApaWindowGroupName* wn=CApaWindowGroupName::NewL(CEikonEnv::Static()->WsSession());
	wn->SetHidden(ETrue);
	wn->SetSystem(ETrue);
	wn->SetWindowGroupName(*iWinGroup);
	delete wn;
	
	iSkinSession.Connect(this,CActive::EPriorityIdle);
	iSkinSession.EnableSkinChangeNotify();
	
	iHideTimer=CPeriodic::NewL(CActive::EPriorityHigh);
	iContainer=CAppContainer::NewL(iWinGroup);
	iCount=0;
	iTimer=CPeriodic::NewL(CActive::EPriorityHigh);
	iAnimator=CControlAnimator::NewL(iContainer);
	
	CSensrvChannelFinder* SensrvChannelFinder = CSensrvChannelFinder::NewLC();
	RSensrvChannelInfoList ChannelInfoList;
	CleanupClosePushL( ChannelInfoList );
	TSensrvChannelInfo mySearchConditions;
	mySearchConditions.iChannelDataTypeId = KSensrvChannelTypeIdProximityMonitor;
	SensrvChannelFinder->FindChannelsL(ChannelInfoList,mySearchConditions);
	iSensorInfo=ChannelInfoList[0];
	iSensorActive=EFalse;
	if (iSettings[0]==1)
		{
		iSensrvChannel = CSensrvChannel::NewL( iSensorInfo );
		iSensrvChannel->OpenChannelL();
		iSensrvChannel->StartDataListeningL( this, 1,1,0);
		iSensorActive=ETrue;
		}
	ChannelInfoList.Close();
	CleanupStack::Pop( &ChannelInfoList );
	CleanupStack::PopAndDestroy( SensrvChannelFinder );
	HideApplicationFromFSW(ETrue);
	
	//CRepository* CR=CRepository::NewL(TUid::Uid(0x101F877C));
	//CR->Get(0xb,iInactivityPeriod);
	//delete CR;
	iInactivityPeriod=20;
	iInactivityTimer=CPeriodic::NewL(CActive::EPriorityHigh);
	iInactivityTimer->Start(KCheckInactivityPeriod*1000,KCheckInactivityPeriod*1000,TCallBack(CheckInactivity,this));
	iLockMonitor=CCenRepObserver::NewL(*this);
	iLockMonitor->StartObservingL();
	
	UpdateSettings();
	iContainer->DrawNow();
	
	iKeyMon=CFgrObserver::NewL(*iWsSession,*this);
	iKeyMon->CaptureKey(168);
	iKeyMon->CaptureKey(169);
	iKeyMon->Listen();
	}
TInt CquickLauncherAppUi::CheckInactivity(TAny* aObj)
	{
	CquickLauncherAppUi* obj=((CquickLauncherAppUi*)aObj);
	if (User::InactivityTime().Int()<KCheckInactivityPeriod*1000&&(!obj->iSensorActive))
		{
		if (obj->iSettings[0]==1)
			{
			if ((!obj->iSensorActive)&&(!obj->iSliderOpened)&&(!obj->iExceptionFocused)&&(!obj->iLocked))
				{
				_LOG(_L("Enable sensor by inactivity"));
				obj->iSensrvChannel=CSensrvChannel::NewL(obj->iSensorInfo);
				obj->iSensrvChannel->OpenChannelL();
				obj->iSensrvChannel->StartDataListeningL( obj, 1,1,0);
				obj->iSensorActive=ETrue;
				}
			}
		}
	else if ((User::InactivityTime().Int()>obj->iInactivityPeriod)&&obj->iSensorActive)
		{
		if (obj->iSensorActive)
			{
			_LOG(_L("Disable sensor by inactivity"));
			obj->iSensrvChannel->StopDataListening();
			obj->iSensrvChannel->CloseChannel();
			if (obj->iSensrvChannel) {delete obj->iSensrvChannel;}
			obj->iSensorActive=EFalse;
			}
		}
	return 1;
	}

void CquickLauncherAppUi::ReadSettings()
	{
	_LOG(_L("CquickLauncherAppUi::ReadSettings()"));
	TInt err=KErrNone;
	iSettings.Reset();
	if (BaflUtils::FileExists(CEikonEnv::Static()->FsSession(),KSettingPath)) //!!!!!!!!!!!!!!!!!!!!!!!!!!!
		{
		TBuf<255> val;
		RFile filesave;
		TBuf<10> t;
		TFileText ft;
		TUint32 IntVal;
		filesave.Open(CEikonEnv::Static()->FsSession(), KSettingPath, EFileRead);
		ft.Set(filesave);
		while (ft.Read(val)==KErrNone)
			{
			TLex conv(val);
			conv.Val(IntVal,EDecimal);
			iSettings.Append(IntVal);
			}
		filesave.Close();
		}
	else
		{
		TParse parse;
		CEikonEnv::Static()->FsSession().Parse(KSettingPath,parse);
		if (!BaflUtils::FolderExists(CEikonEnv::Static()->FsSession(),parse.DriveAndPath()))
			{
			CEikonEnv::Static()->FsSession().MkDirAll(parse.DriveAndPath());
			}
		iSettings.Append(KApplicationWork);
		iSettings.Append(KAutoStart);
		iSettings.Append(KPositionLR);
		iSettings.Append(KApp1);
		iSettings.Append(KApp2);
		iSettings.Append(KApp3);
		iSettings.Append(KApp4);
		iSettings.Append(KApp5);
		WriteSettings();
		}
	}
void CquickLauncherAppUi::WriteSettings()
	{
	_LOG(_L("CquickLauncherAppUi::WriteSettings()"));
		RFs iFS=CEikonEnv::Static()->FsSession();
		RFile filesave;
		TInt err1=filesave.Replace(iFS, KSettingPath, EFileWrite);
		TFileText ft;
		ft.Set(filesave);
		TBuf<255> temp;
		for (TInt i=0;i<iSettings.Count();i++)
			{
			temp.Num(iSettings[i],EDecimal);
			ft.Write(temp);
			}
		filesave.Close();
	}
void CquickLauncherAppUi::ShowContainer(TBool aVisible,TBool aAnimate)
	{
	_LOGDATA(_L("CquickLauncherAppUi::ShowContainer(%d)"),aVisible);
	if (iContainer->CountComponentControls()==0)
		{
		if (!aVisible){return;}
		_LOG(_L("No apps!"));
		TBuf<255> t;
		CEikonEnv::Static()->ReadResource(t,R_NO_APPS);
		CAknGlobalNote* Note=CAknGlobalNote::NewL();
		TRequestStatus status=KRequestPending;
		Note->ShowNoteL(status,EAknGlobalInformationNote,t);
		User::WaitForRequest(status);
		delete Note;
		}
	else
		{
		/*
		TPoint target;
		TMoveDirection dir;
		//iContainer->SetSize(TSize(KButtonSize.iWidth,(KBetween+KButtonSize.iHeight)*iContainer->CountComponentControls()));
		if (iSettings[2]==0)
			{//left
			iContainer->SetPosition(TPoint(0,0));
			iContainer->SetSize(TSize(KButtonSize.iWidth,(KBetween+KButtonSize.iHeight)*iContainer->CountComponentControls()));
			target=TSize(KButtonSize.iWidth,(KBetween+KButtonSize.iHeight)*iContainer->CountComponentControls());
			dx=0; dy=10;
			}
		else if (iSettings[2]==1)
			{//right
			iContainer->SetSize(TSize(KButtonSize.iWidth,(KBetween+KButtonSize.iHeight)*iContainer->CountComponentControls()));
			iContainer->SetPosition(TPoint(CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iWidth-KButtonSize.iWidth,0));
			target=TSize(KButtonSize.iWidth,(KBetween+KButtonSize.iHeight)*iContainer->CountComponentControls());
			dx=0; dy=10;
			}
		else if (iSettings[2]==2)
			{ //horizontal up;
			iContainer->SetSize(TSize((KBetween+KButtonSize.iHeight)*iContainer->CountComponentControls(),KButtonSize.iHeight));
			iContainer->SetPosition(TPoint(0,0));
			target=TSize((KBetween+KButtonSize.iHeight)*iContainer->CountComponentControls(),KButtonSize.iHeight);
			dy=0; dx=10;
			}
		else if (iSettings[2]==3)
			{ //horizontal up;
			iContainer->SetSize(TSize((KBetween+KButtonSize.iHeight)*iContainer->CountComponentControls(),KButtonSize.iHeight));
			iContainer->SetPosition(TPoint(0,CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iHeight-KButtonSize.iHeight));
			target=TSize((KBetween+KButtonSize.iHeight)*iContainer->CountComponentControls(),KButtonSize.iHeight);
			dy=0; dx=10;
			}
		iContainer->MakeVisible(aVisible);
		iContainer->iHider->MakeVisible(aVisible);
		
		TSize cur=iContainer->Size();
		TInt max=Max(target.iWidth,target.iHeight);
		TInt dd=Max(dx,dy);
		TInt count=max/dd;
		_LOGDATA2(_L("dx=%d,dy=%d"),dx,dy);
		_LOGDATA2(_L("Target(%d,%d)"),target.iWidth,target.iWidth);
		_LOGDATA(_L("count=%d"),count);
		for (TInt i=0;i<count;i++)
			{
			iContainer->SetSize(TSize(cur.iWidth+dx,cur.iHeight+dy));
			iContainer->DrawNow();
			}
		*/	
		
		TPoint target;
		TMoveDirection dir;
		if (iSettings[2]==0)
			{//left
			target=TPoint(0,0);
			iContainer->SetSize(TSize(KButtonSize.iWidth,(KBetween+KButtonSize.iHeight)*iContainer->CountComponentControls()));
			dir=EFromUp;
			}
		else if (iSettings[2]==1)
			{//right
			iContainer->SetSize(TSize(KButtonSize.iWidth,(KBetween+KButtonSize.iHeight)*iContainer->CountComponentControls()));
			target=TPoint(CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iWidth-KButtonSize.iWidth,0);
			dir=EFromUp;
			}
		else if (iSettings[2]==2)
			{ //horizontal up;
			iContainer->SetSize(TSize((KBetween+KButtonSize.iHeight)*iContainer->CountComponentControls(),KButtonSize.iHeight));
			target=TPoint(CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iWidth/2-iContainer->Size().iWidth/2,0);
			dir=EFromUp;
			}
		else if (iSettings[2]==3)
			{ //horizontal down;
			iContainer->SetSize(TSize((KBetween+KButtonSize.iHeight)*iContainer->CountComponentControls(),KButtonSize.iHeight));
			target=TPoint(CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iWidth/2-iContainer->Size().iWidth/2,CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iHeight-KButtonSize.iHeight);
			dir=EFromDown;
			}
		if (aAnimate)
			{
			if (aVisible)
				{iAnimator->AnimateShow(target,dir);}
			else
				{iAnimator->AnimateHide(dir);}
			}
		else
			{
			iContainer->SetPosition(target);
			iContainer->MakeVisible(aVisible);
			}
		iContainer->iHider->MakeVisible(aVisible);
		
		
		if (iContainer->iHider->IsVisible()) //iHider because timer in iAnimator, iContainer->Visible() may be true
			{
			iHideTimer->Cancel();
			iHideTimer->Start(KHidePeriod*1000,KHidePeriod*1000,TCallBack(HideCB,this));
			}
		}
	iContainer->DrawNow();
	}
void CquickLauncherAppUi::UpdateSettings()
	{
	_LOG(_L("UpdateSettings()"));
	CheckInactivity(this);
	iContainer->Clear();
	for (TInt i=3;i<iSettings.Count();i++)
		{
		if (iSettings[i]!=0)
			{iContainer->AddApp(TUid::Uid(iSettings[i]));}
		}
	if (iContainer->IsVisible())
		{
		iContainer->SetSize(TSize(KButtonSize.iWidth,(KBetween+KButtonSize.iHeight)*iContainer->CountComponentControls()));
		if (iSettings[2]==0)
			{iContainer->SetPosition(TPoint(0,0));}//left
		else
			{iContainer->SetPosition(TPoint(CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iWidth-KButtonSize.iWidth,0));}
		}
	iContainer->DrawNow();
	}
TBool CquickLauncherAppUi::ProcessCommandParametersL(CApaCommandLine& aCmd)
	{
	_LOG(_L("CquickLauncherAppUi::ProcessCommandParametersL"));
	if (aCmd.OpaqueData().Length()>0)
		{
		//menu
		_LOG(_L("from menu"));
		}
	else
		{
#ifndef _DEBUG
		_LOG(_L("Autostart"));
		if (iSettings[1]==1)
			{_LOG(_L("Hide"));CEikonEnv::Static()->RootWin().SetOrdinalPosition(-4);}
		else
			{_LOG(_L("Exit"));Exit();}
#endif
		}
	return CAknAppUi::ProcessCommandParametersL(aCmd);
	}

void CquickLauncherAppUi::DataReceived( CSensrvChannel& aChannel, TInt aCount, TInt aDataLost )
    {
	_LOG(_L("DataReceived"));
    if ( aChannel.GetChannelInfo().iChannelType ==  KSensrvChannelTypeIdProximityMonitor )
        {
        TSensrvProximityData data;
        for( TInt i = 0; i < aCount; i++ )
            {
            TPckgBuf<TSensrvProximityData> dataBuf;
            aChannel.GetData( dataBuf );
            data = dataBuf();
            TSensrvProximityData::TProximityState state = data.iProximityState;
            if ( iLastState != state )
                {
                if( state == TSensrvProximityData::EProximityIndiscernible )
                    {
                    //button up
                    SensorPressed( EFalse );
                }
                if( state == TSensrvProximityData::EProximityDiscernible )
                    {
                    //button down
                    SensorPressed( ETrue );
                    }
                }
            iLastState = state;
            }
        }
    //dataRequested = EFalse;
    }
void CquickLauncherAppUi::SensorPressed(TBool aDown)
	{
	_LOGDATA(_L("CquickLauncherAppUi::SensorPressed(%d)"),aDown);
	/* old solution
	if (aDown)
		{
		iCount=0;
		if (iTimer->IsActive()){iTimer->Cancel();}
		iTimer->Start(1000000,1000000,TCallBack(Tick,this));
		}
	iCount++;
	*/
	if (aDown){iTimeSensorPressed.HomeTime();}
	else {iTimeSensorReleased.HomeTime();}
	if (!aDown)
		{
		if ((Abs((TInt)iTimeSensorPressed.MicroSecondsFrom(iTimeSensorReleased).Int64())>iMinPeriod*1000)&&((Abs((TInt)iTimeSensorPressed.MicroSecondsFrom(iTimeSensorReleased).Int64())<iMaxPeriod*1000)))
			{
			ShowContainer(!iContainer->IsVisible(),ETrue);
			}
		}
	}
TInt CquickLauncherAppUi::Tick(TAny* aObj)
	{
	/*
	CquickLauncherAppUi* obj=((CquickLauncherAppUi*)aObj);
	_LOGDATA(_L("Tick show. iCount=%d"),obj->iCount);
	if (obj->iTimer->IsActive()){obj->iTimer->Cancel();}
	if (obj->iCount==2)
		{obj->ShowContainer(!obj->iContainer->IsVisible(),ETrue);}
	obj->iCount=0;
	return 0;
	*/
	}
TInt CquickLauncherAppUi::HideCB(TAny* aObj)
	{
	_LOG(_L("Hide by timer"));
	if (((CquickLauncherAppUi*)aObj)->iHideTimer->IsActive()){((CquickLauncherAppUi*)aObj)->iHideTimer->Cancel();}
	((CquickLauncherAppUi*)aObj)->ShowContainer(EFalse,ETrue);
	return 0;
	}
void CquickLauncherAppUi::DataError(CSensrvChannel& aChannel, enum TSensrvErrorSeverity aErr)
	{
	
	}
void CquickLauncherAppUi::GetDataListenerInterfaceL( TUid aInterfaceUid, TAny*& aInterface )
	{
	
	}
void CquickLauncherAppUi::SkinConfigurationChanged(enum TAknsSkinStatusConfigurationChangeReason aReason)
	{
	
	}
void CquickLauncherAppUi::SkinContentChanged()
	{
	UpdateSettings();
	}
void CquickLauncherAppUi::SkinPackageChanged(enum TAknsSkinStatusPackageChangeReason aReason)
	{
	
	}

void CquickLauncherAppUi::ValueChanged(TInt aVal)
	{
	if (aVal==0)
		{
		_LOG(_L("Enable sensor by block"));
		if (iSettings[0]==1)
			{
			if ((iSettings[0]==1)&&(!iSensorActive)&&(!iSliderOpened)&&(!iExceptionFocused))
				{
				iSensrvChannel=CSensrvChannel::NewL(iSensorInfo);
				iSensrvChannel->OpenChannelL();
				iSensrvChannel->StartDataListeningL( this, 1,1,0);
				iSensorActive=ETrue;
				iLocked=EFalse;
				}
			}
		}
	else if (aVal==1)
		{
		if (iSensorActive)
			{
			_LOG(_L("Disable sensor by block"));
			iSensrvChannel->StopDataListening();
			iSensrvChannel->CloseChannel();
			if (iSensrvChannel) {delete iSensrvChannel;}
			iSensorActive=EFalse;
			iLocked=ETrue;
			}
		}
	}

void CquickLauncherAppUi::KeyCaptured(TWsEvent aEvent)
	{
	_LOGDATA(_L("KeyCaptured %d"),aEvent.Key()->iScanCode);
	if (aEvent.Type()==EEventKey||aEvent.Type()==EEventKeyUp||aEvent.Type()==EEventKeyDown)
		{
		if (aEvent.Key()->iScanCode==168)
			{
			if (iSensorActive&&(!iSliderOpened))
				{
				_LOG(_L("Disable sensor by slider"));
				iSensrvChannel->StopDataListening();
				iSensrvChannel->CloseChannel();
				if (iSensrvChannel) {delete iSensrvChannel;}
				iSensorActive=EFalse;
				iSliderOpened=ETrue;
				}
			}
		if (aEvent.Key()->iScanCode==169)
			{
			if ((iSettings[0]==1)&&(!iSensorActive)&&(!iExceptionFocused)&&(!iLocked))
				{
				_LOG(_L("Enable sensor by slider"));
				iSensrvChannel=CSensrvChannel::NewL(iSensorInfo);
				iSensrvChannel->OpenChannelL();
				iSensrvChannel->StartDataListeningL( this, 1,1,0);
				iSensorActive=ETrue;
				iSliderOpened=EFalse;
				}
			}
		}
	}
void CquickLauncherAppUi::FocusChanged(TUid aUid)
	{
	_LOGDATA(_L("focus changed %d"),aUid.iUid);
	if (iExceptions.Find(aUid)!=KErrNotFound)
		{
		if (iSensorActive)
			{
			_LOG(_L("Disable sensor by exception"));
			iSensrvChannel->StopDataListening();
			iSensrvChannel->CloseChannel();
			if (iSensrvChannel) {delete iSensrvChannel;}
			iSensorActive=EFalse;
			iExceptionFocused=ETrue;
			}
		}
	else
		{
		if ((iSettings[0]==1)&&(!iSensorActive)&&(!iSliderOpened)&&(!iLocked))
			{
			_LOG(_L("Enable sensor by exception"));
			iSensrvChannel=CSensrvChannel::NewL(iSensorInfo);
			iSensrvChannel->OpenChannelL();
			iSensrvChannel->StartDataListeningL( this, 1,1,0);
			iSensorActive=ETrue;
			iExceptionFocused=EFalse;
			}
		}
	}
void CquickLauncherAppUi::WriteExceptions()
	{
		CEikonEnv::Static()->InfoMsg(_L("CreatingFile"));
		RFs iFS=CEikonEnv::Static()->FsSession();
		RFile filesave;
		TInt err1=filesave.Replace(iFS, KExceptionsPath, EFileWrite);
		TFileText ft;
		ft.Set(filesave);
		TInt i;
		TBuf<255> t;
		CEikonEnv::Static()->InfoMsg(_L("StartWriting"));
		for (i=0;i<iExceptions.Count();i++)
			{
			t.Num(iExceptions[i].iUid,EHex);
			ft.Write(t);
			}
		filesave.Close();
		CEikonEnv::Static()->InfoMsg(_L("EndWriting"));
	}
void CquickLauncherAppUi::ReadExceptions()
	{
	TInt err=KErrNone;
	iExceptions.Reset();
	if (BaflUtils::FileExists(CEikonEnv::Static()->FsSession(),KExceptionsPath)) //!!!!!!!!!!!!!!!!!!!!!!!!!!!
		{
		TBuf<255> val;
		TLex conv;
		TUint32 IntVal;
		RFile filesave;
		TBuf<10> t;
		TFileText ft;
		filesave.Open(CEikonEnv::Static()->FsSession(), KExceptionsPath, EFileRead);
		ft.Set(filesave);
		while (ft.Read(val)==KErrNone)
			{
			conv.Assign(val);
			conv.Val(IntVal,EHex);
			iExceptions.AppendL(TUid::Uid(IntVal));
			}
		filesave.Close();
		}
	else
		{
		TParse parse;
		CEikonEnv::Static()->FsSession().Parse(KExceptionsPath,parse);
		if (!BaflUtils::FolderExists(CEikonEnv::Static()->FsSession(),parse.DriveAndPath()))
			{
			CEikonEnv::Static()->FsSession().MkDirAll(parse.DriveAndPath());
			}
		}
	}
void CquickLauncherAppUi::ReadPeriods()
	{
	if (BaflUtils::FileExists(CEikonEnv::Static()->FsSession(),KPeriodPath))
		{
		TBuf<255> val;
		TLex conv;
		TUint32 IntVal;
		RFile filesave;
		TBuf<10> t;
		TFileText ft;
		filesave.Open(CEikonEnv::Static()->FsSession(), KPeriodPath, EFileRead);
		ft.Set(filesave);
		ft.Read(val);
		conv.Assign(val);
		conv.Val(iMinPeriod);
		ft.Read(val);
		conv.Assign(val);
		conv.Val(iMaxPeriod);
		filesave.Close();
		if (iMinPeriod<0){iMinPeriod=KMinPressPeriod;}
		if (iMaxPeriod<0){iMaxPeriod=KMaxPressPeriod;}
		}
	else
		{
		iMinPeriod=KMinPressPeriod;
		iMaxPeriod=KMaxPressPeriod;
		}
	}
void CquickLauncherAppUi::HandleForegroundEventL(TBool aForeground)
	{
	if (aForeground){ReadPeriods();}
	}
