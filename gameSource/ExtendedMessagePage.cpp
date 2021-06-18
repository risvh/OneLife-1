#include "ExtendedMessagePage.h"

#include "buttonStyle.h"
#include "message.h"

#include "minorGems/game/Font.h"
#include "minorGems/game/game.h"

#include "minorGems/util/stringUtils.h"


extern Font *mainFont;


extern char *userEmail;
extern char *accountKey;

extern bool useDarkMode;





ExtendedMessagePage::ExtendedMessagePage()
        : mOKButton( mainFont, 0, -128, 
                     translate( "okay" ) ),
          mRebornButton( mainFont, 150, -128, 
                         translate( "reborn" ) ),
          mGenesButton( mainFont, -300, 64, 
                        translate( "geneticHistoryButton" ) ),
          mMenuButton( mainFont, -522, 300, 
                       translate( "menu" ) ),
          mQuitButton( mainFont, -150, -128, 
                       translate( "quit" ) ),
          mMessageKey( "" ),
          mSubMessage( NULL ) {

    addComponent( &mOKButton );
    
    setButtonStyle( &mOKButton );
    
    mOKButton.addActionListener( this );
	
	addComponent( &mRebornButton );
	addComponent( &mGenesButton );
	addComponent( &mMenuButton );
	addComponent( &mQuitButton );
	
	setButtonStyle( &mRebornButton );
	setButtonStyle( &mGenesButton );
	setButtonStyle( &mMenuButton );
	setButtonStyle( &mQuitButton );
	
    mRebornButton.addActionListener( this );
    mGenesButton.addActionListener( this );
    mMenuButton.addActionListener( this );
    mQuitButton.addActionListener( this );
    }


ExtendedMessagePage::~ExtendedMessagePage() {
    if( mSubMessage != NULL ) {
        delete [] mSubMessage;
        }
    }




void ExtendedMessagePage::setMessageKey( const char *inMessageKey ) {
    mMessageKey = inMessageKey;
    }

void ExtendedMessagePage::setSubMessage( const char *inMessage ) {
    if( mSubMessage != NULL ) {
        delete [] mSubMessage;
        }
    mSubMessage = stringDuplicate( inMessage );
    }



        
void ExtendedMessagePage::actionPerformed( GUIComponent *inTarget ) {
    if( inTarget == &mOKButton ) {
        setSignal( "done" );
        }
    else if( inTarget == &mRebornButton ) {
        setSignal( "reborn" );
        }
    else if( inTarget == &mGenesButton ) {
        setSignal( "genes" );
        }
    else if( inTarget == &mMenuButton ) {
        setSignal( "menu" );
        }
    else if( inTarget == &mQuitButton ) {
        setSignal( "quit" );
        }
    }



void ExtendedMessagePage::draw( doublePair inViewCenter, 
                                  double inViewSize ) {
    
    doublePair pos = { 0, 200 };
	
	float fade = 1.0;
	if ( useDarkMode ) fade = 0.5;
    
    drawMessage( mMessageKey, pos, false, fade );
    
    if( mSubMessage != NULL ) {
        pos.y = 50;
        drawMessage( mSubMessage, pos, false, fade );
        }
    
    }


void ExtendedMessagePage::makeActive( char inFresh ) {
	updateOnDarkMode();
	}


void ExtendedMessagePage::updateOnDarkMode() {

	if ( useDarkMode && strcmp( mMessageKey, "connectionLost" ) != 0 ) {
		
		mOKButton.setVisible( false );
		
		mRebornButton.setPosition( 0, -160 );
		mGenesButton.setPosition( 0, -240 );
		mMenuButton.setPosition( 0, -320 );
		mQuitButton.setPosition( 0, -400 );
		
		mRebornButton.setVisible( true );
		mGenesButton.setVisible( true );
		mMenuButton.setVisible( true );
		mQuitButton.setVisible( true );

	} else {
		
		mOKButton.setVisible( true );
		
		mRebornButton.setVisible( false );
		mGenesButton.setVisible( false );
		mMenuButton.setVisible( false );
		mQuitButton.setVisible( false );

	}
	
}