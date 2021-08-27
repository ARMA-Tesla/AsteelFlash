#if _NI_mswin32_
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif /* _NI_mswin32 */
//#include <formatio.h>
#include <utility.h>
//#include <ansi_c.h>
#include <userint.h>
#include <formatio.h>
#include "toolbox.h"
#include "pwctrl.h"


#define DEFAULT_MASK_CHARACTER  '*'
#define PASSWORD_CTRL_TYPE      "Password Control"

typedef struct
    {
    int                 panel;
    int                 ctrl;
    int                 maxPasswordLength;
    char                password[MAX_POSSIBLE_PASSWORD_LENGTH+1];
    char                maskCharacter;
    int                 valid;                                    /* Tells if the lock is good. */
    int                 lockHandle;
    } *PasswordInfo;

static int  UpdateObscuringCharacters(int panel, int passwordCtrl);
static int  CreatePasswordInfo(int panel, int ctrl, PasswordInfo *newPasswordInfo, int *pLocked);
static void DiscardPasswordInfo(PasswordInfo passwordInfo, int *pLocked);

int  CVICALLBACK PasswordCtrl_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);

/*****************************************************************************************/

static int UpdateObscuringCharacters(int panel, int passwordCtrl)
{
    int             error = UIENoError;
    char            maskBuffer[MAX_POSSIBLE_PASSWORD_LENGTH+1];
    int             strLength;
    PasswordInfo    pwInfo = 0;

    errChk( GetChainedCallbackData(panel, passwordCtrl, PASSWORD_CTRL_TYPE, (void **)&pwInfo));

    strLength = strlen(pwInfo->password);
    memset (maskBuffer, pwInfo->maskCharacter, strLength);
    maskBuffer[strLength] = 0;
    errChk(SetCtrlVal(panel, passwordCtrl, maskBuffer));

Error:
    return error;
}

/*****************************************************************************************/

int CVIFUNC PasswordCtrl_Create(int panel, const char *label, int top, int left)
{
    int stringCtrl;
    int error  = 0;

    errChk(stringCtrl = NewCtrl (panel, CTRL_STRING, label, top, left));

    error = PasswordCtrl_ConvertFromString(panel, stringCtrl);

Error:
    if (error < 0)
        DiscardCtrl(panel, stringCtrl);
    return error;
}

/*****************************************************************************************/

int CVIFUNC PasswordCtrl_ConvertFromString(int panel, int stringCtrl)
{
    int             error = UIENoError;
    char            password[MAX_POSSIBLE_PASSWORD_LENGTH+1];
    PasswordInfo    pwInfo = 0;
    int             locked = FALSE;    
    
    errChk( CreatePasswordInfo(panel, stringCtrl, &pwInfo, &locked));       /* Creates and gets the lock. */
    
    PasswordCtrl_SetAttribute(panel, stringCtrl, ATTR_PASSWORD_MASK_CHARACTER, DEFAULT_MASK_CHARACTER);
    PasswordCtrl_SetAttribute(panel, stringCtrl, ATTR_PASSWORD_MAX_LENGTH, MAX_POSSIBLE_PASSWORD_LENGTH);

    GetCtrlVal(panel, stringCtrl, password);
    errChk( PasswordCtrl_SetAttribute(panel, stringCtrl, ATTR_PASSWORD_VAL, password) );

Error:
    if (error < 0)
        DiscardPasswordInfo(pwInfo, &locked);                               /* Releases and deletes the lock. */
    else if (locked)
        CmtReleaseLock(pwInfo->lockHandle);

    return (error < 0) ? error : stringCtrl;
}

/*****************************************************************************************/

int CVIFUNC PasswordCtrl_RevertToString(int panel, int control)
{
    int             error = UIENoError;
    int             locked = FALSE;
    PasswordInfo    pwInfo = 0;
    
    errChk( GetChainedCallbackData(panel, control, PASSWORD_CTRL_TYPE, (void **)&pwInfo));
    if (!pwInfo->valid)
        errChk(UIEInvalidControlId);
    
    DiscardPasswordInfo(pwInfo, &locked);                               /* Releases and deletes the lock. */
Error:
    return (error < 0) ? error : control;
}

/*****************************************************************************************/

static int CreatePasswordInfo(int panel, int ctrl, PasswordInfo *newPasswordInfo, int *pLocked)
{
    int             error = UIENoError;
    PasswordInfo    pwInfo = 0;
    int             locked = FALSE;    

    nullChk(pwInfo = (PasswordInfo)calloc(sizeof(*pwInfo), 1));
    errChk( CmtNewLock(NULL, 0, &pwInfo->lockHandle) );
    errChk( CmtGetLock(pwInfo->lockHandle) );
    locked = TRUE;
    pwInfo->valid = TRUE;

    pwInfo->panel = panel;
    pwInfo->ctrl = ctrl;

    errChk( ChainCtrlCallback(panel, ctrl, PasswordCtrl_Callback, (void *)pwInfo, PASSWORD_CTRL_TYPE));

    if (newPasswordInfo)
        *newPasswordInfo = pwInfo;

Error:
    /* Do not release lock, calling function still needs it to be locked. */
    if (error < 0)
        {
        if (newPasswordInfo)
            *newPasswordInfo = 0;
        }
        
    *pLocked = locked;      
    return error;
}

/*****************************************************************************************/

static void DiscardPasswordInfo(PasswordInfo passwordInfo, int *pLocked)
{
    if (passwordInfo && passwordInfo->valid)
        {
        passwordInfo->valid = FALSE;
        if (!(*pLocked))
            CmtGetLock(passwordInfo->lockHandle);       /* Want to make sure the lock is not owned by another thread. */

        CmtReleaseLock(passwordInfo->lockHandle);

        *pLocked = FALSE;
        CmtDiscardLock(passwordInfo->lockHandle);
        UnchainCtrlCallback(passwordInfo->panel, passwordInfo->ctrl, PASSWORD_CTRL_TYPE);
        free(passwordInfo);
        }
}

/*****************************************************************************************/

int CVIFUNC_C PasswordCtrl_SetAttribute(int panel, int passwordCtrl, int attribute, ...)
{
    int             error = UIENoError;
    va_list         parmInfo;
    PasswordInfo    pwInfo = 0;
    int             locked = FALSE;
    int             vaStarted = FALSE;
    
    errChk( GetChainedCallbackData(panel, passwordCtrl, PASSWORD_CTRL_TYPE, (void **)&pwInfo));
    
    if (!pwInfo->valid)
        errChk(UIEInvalidControlId);
    
    errChk( CmtGetLock(pwInfo->lockHandle) );
    locked = TRUE;

    va_start(parmInfo, attribute);
    vaStarted = TRUE;

    switch (attribute)
        {
        case ATTR_PASSWORD_MAX_LENGTH:
            pwInfo->maxPasswordLength = va_arg(parmInfo, int);

            if (pwInfo->maxPasswordLength < 0)
                pwInfo->maxPasswordLength = 0;
            else
            if (pwInfo->maxPasswordLength > MAX_POSSIBLE_PASSWORD_LENGTH)
                pwInfo->maxPasswordLength = MAX_POSSIBLE_PASSWORD_LENGTH;

            PasswordCtrl_SetAttribute(panel, passwordCtrl, ATTR_PASSWORD_VAL, "");
            SetCtrlAttribute(panel, passwordCtrl, ATTR_MAX_ENTRY_LENGTH, pwInfo->maxPasswordLength);
            pwInfo->password[pwInfo->maxPasswordLength] = 0;    /* truncate any existing password */
            break;
        case ATTR_PASSWORD_VAL:
            {
            char    *newPasswordVal = (char *)va_arg(parmInfo, char *);

                if (!newPasswordVal)
                    newPasswordVal = "";

                strncpy (pwInfo->password, newPasswordVal, pwInfo->maxPasswordLength);
                UpdateObscuringCharacters(panel, passwordCtrl);
            }
            break;
        case ATTR_PASSWORD_MASK_CHARACTER:
            pwInfo->maskCharacter = va_arg(parmInfo, char);
            if (pwInfo->maskCharacter == 0)
                pwInfo->maskCharacter = DEFAULT_MASK_CHARACTER;

            UpdateObscuringCharacters(panel, passwordCtrl);
            break;
        default:
            errChk(UIEInvalidAttribute);
            break;
        }

Error:
    if (vaStarted)
        va_end(parmInfo);
    if (locked)
        CmtReleaseLock(pwInfo->lockHandle);
    return error;
}

/*****************************************************************************************/

int CVIFUNC PasswordCtrl_GetAttribute(int panel, int passwordCtrl, int attribute, void *value)
{
    int             error = UIENoError;
    PasswordInfo    pwInfo = 0;
    int             locked = FALSE;

    if (!value)
        errChk(UIENullPointerPassed);
        
    errChk( GetChainedCallbackData(panel, passwordCtrl, PASSWORD_CTRL_TYPE, (void **)&pwInfo));

    if (!pwInfo->valid)
        errChk(UIEInvalidControlId);

    errChk( CmtGetLock(pwInfo->lockHandle) );
    locked = TRUE;

    switch (attribute)
        {
        case ATTR_PASSWORD_MAX_LENGTH:
            *(int *)value = pwInfo->maxPasswordLength;
             break;
        case ATTR_PASSWORD_VAL:
            if (value)
                strcpy ((char *)value, pwInfo->password);
            break;
        case ATTR_PASSWORD_MASK_CHARACTER:
            *(char *)value = pwInfo->maskCharacter;
            break;
        default:
            errChk(UIEInvalidAttribute);
            break;
        }

Error:
    if (locked)
        CmtReleaseLock(pwInfo->lockHandle);
    return error;
}

/*****************************************************************************************/

int  CVICALLBACK PasswordCtrl_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    PasswordInfo    pwInfo = (PasswordInfo)callbackData;
    int             selectionStart, selectionLength, textLength;
    int             entireTextSelected;
    int             swallowEvent = 0;
    int             locked = FALSE;
    
    if (!pwInfo->valid)              
        return 0;

    CmtGetLock(pwInfo->lockHandle);
    locked = TRUE;

    GetCtrlAttribute(panel, control, ATTR_TEXT_SELECTION_START, &selectionStart);
    GetCtrlAttribute(panel, control, ATTR_TEXT_SELECTION_LENGTH, &selectionLength);
    GetCtrlAttribute(panel, control, ATTR_STRING_TEXT_LENGTH, &textLength);

    entireTextSelected = textLength > 0 && textLength == selectionLength;

        /* is the entire text is not selected, the cursor must be at the end of the password */
    if (selectionStart != textLength && !entireTextSelected)
        {
        selectionStart = textLength;
        selectionLength = 0;
        SetCtrlAttribute(panel, control, ATTR_TEXT_SELECTION_START, selectionStart);
        SetCtrlAttribute(panel, control, ATTR_TEXT_SELECTION_LENGTH, selectionLength);
        }

    switch (event)
        {
        case EVENT_DISCARD:
            DiscardPasswordInfo(pwInfo, &locked);    /* Releases and deletes the lock. */
            break;
        case EVENT_LEFT_CLICK:
        case EVENT_LEFT_DOUBLE_CLICK:
            SetActivePanel(panel);
            SetActiveCtrl(panel, control);
            swallowEvent = 1;   /* Swallow clicks that move the cursor from the end */
            break;
        case EVENT_KEYPRESS:
            {
            int virtualKey;
            int asciiCode;

            virtualKey = eventData1 & VAL_VKEY_MASK;
            asciiCode = eventData1 & VAL_ASCII_KEY_MASK;

            if (eventData1 & VAL_MENUKEY_MODIFIER)  /* disallow cut & paste, etc.. */
                {
                swallowEvent = 1;
                break;
                }

            if  (virtualKey != 0)
                {
                switch (virtualKey)
                    {
                    case VAL_BACKSPACE_VKEY:  /* Check for Backspace Key */
                        if (entireTextSelected)
                            pwInfo->password[0] = 0;        /* delete whole password */
                        else
                        if (selectionStart > 0)
                          pwInfo->password[selectionStart-1] = '\0';    /* Remove one character from password */
                        break;
                    case VAL_FWD_DELETE_VKEY:
                    case VAL_INSERT_VKEY:
                    case VAL_UP_ARROW_VKEY:
                    case VAL_DOWN_ARROW_VKEY:
                    case VAL_LEFT_ARROW_VKEY:
                    case VAL_HOME_VKEY:
                        swallowEvent = 1;                   /* swallow keys that move the cursor from the end */
                        break;
                    case VAL_RIGHT_ARROW_VKEY:
                    case VAL_END_VKEY:
                        swallowEvent = !entireTextSelected;     /* no point in allowing unless all text is selected */
                        break;
                    }
                }
            else
            if (asciiCode)
                {
                if (selectionStart < pwInfo->maxPasswordLength)  /* Check password index */
                    {
                    if (entireTextSelected)
                        pwInfo->password[0] = 0;    /* type over entire password since it was selected */
                    /* This is multibyte safe; we are just adding each byte of a multibyte character separately */
                    Fmt(pwInfo->password,"%s[a]<%c", eventData1);   /* Store the entered keystroke  */
                    *(int*)(eventData2) = pwInfo->maskCharacter;
                    break;
                    }
                else                                        /* length limit reached */
                    {
                    Beep();                                 /* Beep to notify */
                    swallowEvent = 1;                       /* Return a non-zero to "swallow" KEYPRESS */
                    }
                }
            }
            break;
        }

    if (locked)
        CmtReleaseLock(pwInfo->lockHandle);
    return swallowEvent;
}
