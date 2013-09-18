/*
========================================================================
 Name        : quickLauncherDocument.h
 Author      : Usanov-Kornilov Nikolay (aka Kolay)
 Copyright   : 
Contacts:
	kolayuk@mail.ru
	http://kolaysoft.ru

(c) KolaySoft, 2011

 Description : 
========================================================================
*/
#ifndef QUICKLAUNCHERDOCUMENT_H
#define QUICKLAUNCHERDOCUMENT_H

#include <akndoc.h>
		
class CEikAppUi;

/**
* @class	CquickLauncherDocument quickLauncherDocument.h
* @brief	A CAknDocument-derived class is required by the S60 application 
*           framework. It is responsible for creating the AppUi object. 
*/
class CquickLauncherDocument : public CAknDocument
	{
public: 
	// constructor
	static CquickLauncherDocument* NewL( CEikApplication& aApp );

private: 
	// constructors
	CquickLauncherDocument( CEikApplication& aApp );
	void ConstructL();
	void UpdateTaskNameL(CApaWindowGroupName* aWg);
public: 
	// from base class CEikDocument
	CEikAppUi* CreateAppUiL();
	};
#endif // QUICKLAUNCHERDOCUMENT_H
