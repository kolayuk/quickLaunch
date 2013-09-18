/*
========================================================================
 Name        : quickLauncherAppUi.h
 Author      : Usanov-Kornilov Nikolay (aka Kolay)
 Copyright   : 
Contacts:
	kolayuk@mail.ru
	http://kolaysoft.ru

(c) KolaySoft, 2011

 Description : 
========================================================================
*/
#ifndef QUICKLAUNCHERAPPUI_H
#define QUICKLAUNCHERAPPUI_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknviewappui.h>
// ]]] end generated region [Generated Includes]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class CMainView;
// ]]] end generated region [Generated Forward Declarations]

#include <AppContainer.h>
#include <w32std.h>
#include <FgrObserver.h>
#include <sensrvchannel.h>
#include <sensrvproximitysensor.h>
#include <sensrvdatalistener.h>
#include <AknSSrvClient.h>		//aknskinsrv.lib	
#include <CenRepObserver.h>
#include <fgrobserver.h>
#include <ControlAnimator.h>

/**
 * @class	CquickLauncherAppUi quickLauncherAppUi.h
 * @brief The AppUi class handles application-wide aspects of the user interface, including
 *        view management and the default menu, control pane, and status pane.
 */
class CquickLauncherAppUi : public CAknViewAppUi, public MSensrvDataListener, public MAknsSkinChangeObserver, public MCRCallBack, public MFgrCallBack
	{
public: 
	// constructor and destructor
	CquickLauncherAppUi();
	virtual ~CquickLauncherAppUi();
	void ConstructL();
	void ReadSettings();
	void WriteSettings();
	void HandleForegroundEventL(TBool aForeground);
	RArray<TUint> iHandles;
	RArray<TUint32> iSettings;
	void UpdateSettings();
	RArray<TUid> iExceptions;
	void ReadExceptions();
	void WriteExceptions();
	RAknsSrvSession iSkinSession;
	CCenRepObserver* iLockMonitor;
	void ValueChanged(TInt aValue);
    void SkinContentChanged();
    void SkinConfigurationChanged(const TAknsSkinStatusConfigurationChangeReason aReason );
    void SkinPackageChanged(const TAknsSkinStatusPackageChangeReason aReason );
	TBool ProcessCommandParametersL(CApaCommandLine& aCmd);
	CAppContainer* iContainer;
	CPeriodic* iInactivityTimer;
	TInt iInactivityPeriod;
	TBool iSensorActive;
	static TInt CheckInactivity(TAny* aObj);
	RWindowGroup* iWinGroup;
	void ShowContainer(TBool aVisible, TBool aAnimation);
	CPeriodic* iHideTimer;
	static TInt HideCB(TAny* aObj);
	TSensrvChannelInfo iSensorInfo;
	void DataReceived( CSensrvChannel& aChannel, TInt aCount, TInt aDataLost );
	void DataError( CSensrvChannel& aChannel, TSensrvErrorSeverity aError );
	void GetDataListenerInterfaceL( TUid aInterfaceUid, TAny*& aInterface );
	CSensrvChannel* iSensrvChannel;
	TInt iCount;
	CPeriodic* iTimer;
	static TInt Tick(TAny* aObj);
	void SensorPressed(TBool aDown);
	TSensrvProximityData::TProximityState iLastState;
	void KeyCaptured(TWsEvent aEvent);
	void FocusChanged(TUid aUid);
	TBool iExceptionFocused;
	TBool iSliderOpened;
	TBool iLocked;
	CFgrObserver* iKeyMon;
	RWsSession* iWsSession;
	CControlAnimator* iAnimator;
	TTime iTimeSensorPressed;
	TTime iTimeSensorReleased;
	TInt iMinPeriod;
	TInt iMaxPeriod;
	void ReadPeriods();
public:
	// from CCoeAppUi
	TKeyResponse HandleKeyEventL(
				const TKeyEvent& aKeyEvent,
				TEventCode aType );

	// from CEikAppUi
	void HandleCommandL( TInt aCommand );
	void HandleResourceChangeL( TInt aType );

	// from CAknAppUi
	void HandleViewDeactivation( 
			const TVwsViewId& aViewIdToBeDeactivated, 
			const TVwsViewId& aNewlyActivatedViewId );

private:
	void InitializeContainersL();
	// [[[ begin generated region: do not modify [Generated Methods]
public: 
	// ]]] end generated region [Generated Methods]
	
	// [[[ begin generated region: do not modify [Generated Instance Variables]
private: 
	CMainView* iMainView;
	// ]]] end generated region [Generated Instance Variables]
	
	
	// [[[ begin [User Handlers]
protected: 
	// ]]] end [User Handlers]
	
	};

#endif // QUICKLAUNCHERAPPUI_H			
