/*
 * FgrObserver.cpp
 *
 *  Created on: 24.10.2009
 *      Author: Avis
 */

#include "FgrObserver.h"
#include <apgtask.h>
#include <apgwgnam.h> //CApaWindowGroupName
#ifdef _DEBUG
#include <e32debug.h>
#endif
#include <logger.h>

CFgrObserver* CFgrObserver::NewL(RWsSession& aWsSession,MFgrCallBack& aCallBack)
	{
	CFgrObserver* self = CFgrObserver::NewLC(aWsSession,aCallBack);
	CleanupStack::Pop(self);
	return self;
	}
 
 
CFgrObserver* CFgrObserver::NewLC(RWsSession& aWsSession,MFgrCallBack& aCallBack)
	{
	CFgrObserver* self = new (ELeave) CFgrObserver(aWsSession,aCallBack);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}
 
 
CFgrObserver::CFgrObserver(RWsSession& aWsSession,MFgrCallBack& aCallBack)
:CActive(EPriorityHigh),iCallBack(aCallBack), iWsSession(aWsSession), iWg(aWsSession)
{
}
 
CFgrObserver::~CFgrObserver()
{
	Cancel();
	for (TInt i=0;i<iHandles.Count();i++)
		{
		iWg.CancelCaptureKeyUpAndDowns(iHandles[i]);
		}
	iWg.Close();
}
 
void CFgrObserver::ConstructL()
{
	CActiveScheduler::Add(this);
 
	User::LeaveIfError(iWg.Construct((TUint32)&iWg, EFalse));
	iWg.SetOrdinalPosition(-1);
	iWg.EnableReceiptOfFocus(EFalse);
	iWg.EnableFocusChangeEvents();
	CApaWindowGroupName* wn=CApaWindowGroupName::NewLC(iWsSession);
	wn->SetHidden(ETrue);
	wn->SetWindowGroupName(iWg);
	CleanupStack::PopAndDestroy();
}
 
void CFgrObserver::RunL()
	{
		TWsEvent wsEvent;
		iWsSession.GetEvent(wsEvent); 
		_LOGDATA(_L("WsEvent captured, type=%d"),wsEvent.Type());
		switch (wsEvent.Type())
			{
			case EEventKeyUp:
				{
				iCallBack.KeyCaptured(wsEvent);
				TInt wgId = iWsSession.GetFocusWindowGroup();
				iWsSession.SendEventToWindowGroup(wgId, wsEvent);
				break;
				}
			case EEventKeyDown:
				{
				iCallBack.KeyCaptured(wsEvent);
				TInt wgId = iWsSession.GetFocusWindowGroup();
				iWsSession.SendEventToWindowGroup(wgId, wsEvent);
				break;
				}
			case EEventKey:
				{
				iCallBack.KeyCaptured(wsEvent);
				TInt wgId = iWsSession.GetFocusWindowGroup();
				iWsSession.SendEventToWindowGroup(wgId, wsEvent);
				break;
				}			
			case EEventFocusGroupChanged:
				{
				CApaWindowGroupName* wg=CApaWindowGroupName::NewL(iWsSession,iWsSession.GetFocusWindowGroup());
				TUid uid=wg->AppUid();
				delete wg;
				iCallBack.FocusChanged(uid);
				break;
				}
			default:
				//iCallBack.ForegroundEventL();
				break;
			}	 

		if (iStatus != KErrCancel) 
		{
			Listen();
		}
	}
 
void CFgrObserver::DoCancel()
{
	iWsSession.EventReadyCancel();
}
 
void CFgrObserver::Listen()
{
	iWsSession.EventReady(&iStatus);
	SetActive();
}
void CFgrObserver::CaptureKey(TInt aScancode)
	{
	_LOGDATA(_L("Start capturing %d"),aScancode);
	TInt32 handle=iWg.CaptureKeyUpAndDowns(aScancode,0,0);
	_LOGDATA(_L("Capture key handle=%d"),handle);
	if (handle>=0){iHandles.Append(handle);}
	}
