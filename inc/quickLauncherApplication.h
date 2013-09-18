/*
========================================================================
 Name        : quickLauncherApplication.h
 Author      : Usanov-Kornilov Nikolay (aka Kolay)
 Copyright   : 
Contacts:
	kolayuk@mail.ru
	http://kolaysoft.ru

(c) KolaySoft, 2011

 Description : 
========================================================================
*/
#ifndef QUICKLAUNCHERAPPLICATION_H
#define QUICKLAUNCHERAPPLICATION_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknapp.h>
// ]]] end generated region [Generated Includes]

// [[[ begin generated region: do not modify [Generated Constants]
const TUid KUidquickLauncherApplication = { 0x200396A3 };
// ]]] end generated region [Generated Constants]

/**
 *
 * @class	CquickLauncherApplication quickLauncherApplication.h
 * @brief	A CAknApplication-derived class is required by the S60 application 
 *          framework. It is subclassed to create the application's document 
 *          object.
 */
class CquickLauncherApplication : public CAknApplication
	{
private:
	TUid AppDllUid() const;
	CApaDocument* CreateDocumentL();
	
	};
			
#endif // QUICKLAUNCHERAPPLICATION_H		
