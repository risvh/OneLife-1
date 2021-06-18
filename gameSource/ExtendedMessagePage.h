#include "GamePage.h"



#include "minorGems/ui/event/ActionListener.h"


#include "TextButton.h"


class ExtendedMessagePage : public GamePage, public ActionListener {
        
    public:
        ExtendedMessagePage();
        ~ExtendedMessagePage();
        

        void setMessageKey( const char *inMessageKey );
        
        // destroyed by caller
        void setSubMessage( const char *inMessage );
        

        virtual void actionPerformed( GUIComponent *inTarget );

        virtual void draw( doublePair inViewCenter, 
                           double inViewSize );
		
		virtual void makeActive( char inFresh );
		
		virtual void updateOnDarkMode();
        
    protected:
        
        TextButton mOKButton;
		
        TextButton mRebornButton;
        TextButton mGenesButton;
        TextButton mMenuButton;
		TextButton mQuitButton;

        const char *mMessageKey;
        char *mSubMessage;


    };
