//////////////////////////////////////////////////////////////////////////////
// File:        contrib/samples/stc/edit.cpp
// Purpose:     STC test module
// Maintainer:  Wyo
// Created:     2003-09-01
// RCS-ID:      $Id: edit.cpp,v 1.22 2013-07-06 14:44:22 schillacia Exp $
// Copyright:   (c) wxGuide
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// informations
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// headers
//----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all 'standard' wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

//! wxWidgets headers
#include "wx/file.h"     // raw file io support
#include "wx/filename.h" // filename support

//! application headers
#include "defsext.h"     // additional definitions

#include "edit.h"
#include <wx/tokenzr.h>  // string tokenizer

//----------------------------------------------------------------------------
// resources
//----------------------------------------------------------------------------


//============================================================================
// declarations
//============================================================================


//============================================================================
// implementation
//============================================================================

//----------------------------------------------------------------------------
// Edit
//----------------------------------------------------------------------------

BEGIN_EVENT_TABLE (Edit, wxStyledTextCtrl)
    // common
    EVT_SIZE (                         Edit::OnSize)
    // edit
    EVT_MENU (wxID_CLEAR,              Edit::OnEditClear)
    EVT_MENU (wxID_CUT,                Edit::OnEditCut)
    EVT_MENU (wxID_COPY,               Edit::OnEditCopy)
    EVT_MENU (wxID_PASTE,              Edit::OnEditPaste)
    EVT_MENU (myID_INDENTINC,          Edit::OnEditIndentInc)
    EVT_MENU (myID_INDENTRED,          Edit::OnEditIndentRed)
    EVT_MENU (wxID_SELECTALL,          Edit::OnEditSelectAll)
    EVT_MENU (myID_SELECTLINE,         Edit::OnEditSelectLine)
    EVT_MENU (wxID_REDO,               Edit::OnEditRedo)
    EVT_MENU (wxID_UNDO,               Edit::OnEditUndo)
    // find
    EVT_MENU (wxID_FIND,               Edit::OnFind)
    EVT_MENU (myID_FINDNEXT,           Edit::OnFindNext)
    EVT_MENU (myID_REPLACE,            Edit::OnReplace)
    EVT_MENU (myID_REPLACENEXT,        Edit::OnReplaceNext)
    EVT_MENU (myID_BRACEMATCH,         Edit::OnBraceMatch)
    EVT_MENU (myID_GOTO,               Edit::OnGoto)
    // view
    EVT_MENU_RANGE (myID_HILIGHTFIRST, myID_HILIGHTLAST,
                                       Edit::OnHilightLang)
    EVT_MENU (myID_DISPLAYEOL,         Edit::OnDisplayEOL)
    EVT_MENU (myID_INDENTGUIDE,        Edit::OnIndentGuide)
    EVT_MENU (myID_LINENUMBER,         Edit::OnLineNumber)
    EVT_MENU (myID_LONGLINEON,         Edit::OnLongLineOn)
    EVT_MENU (myID_WHITESPACE,         Edit::OnWhiteSpace)
    EVT_MENU (myID_FOLDTOGGLE,         Edit::OnFoldToggle)
    EVT_MENU (myID_OVERTYPE,           Edit::OnSetOverType)
    EVT_MENU (myID_READONLY,           Edit::OnSetReadOnly)
    EVT_MENU (myID_WRAPMODEON,         Edit::OnWrapmodeOn)
    EVT_MENU (myID_CHARSETANSI,        Edit::OnUseCharset)
    EVT_MENU (myID_CHARSETMAC,         Edit::OnUseCharset)
    // extra
    EVT_MENU (myID_CHANGELOWER,        Edit::OnChangeCase)
    EVT_MENU (myID_CHANGEUPPER,        Edit::OnChangeCase)
    EVT_MENU (myID_CONVERTCR,          Edit::OnConvertEOL)
    EVT_MENU (myID_CONVERTCRLF,        Edit::OnConvertEOL)
    EVT_MENU (myID_CONVERTLF,          Edit::OnConvertEOL)
    // stc
    EVT_STC_MARGINCLICK (wxID_ANY,     Edit::OnMarginClick)
    EVT_STC_CHARADDED (wxID_ANY,       Edit::OnCharAdded)
    
    // custom
    EVT_MENU (myID_AUTOCOMPON,         Edit::OnAutocompOn)
    EVT_MENU (myID_HOTKEYSON,          Edit::OnHotkeysOn)
    
END_EVENT_TABLE()

Edit::Edit (wxWindow *parent, wxWindowID id,
            const wxPoint &pos,
            const wxSize &size,
            long style,
            wxString filename,
            MyFrame *frame)
    : wxStyledTextCtrl (parent, id, pos, size, style) {
    // PL: filename e' usato solo per settare il lexer
    m_filename = wxEmptyString;
    m_frame = frame;

    m_LineNrID = 0;
    m_DividerID = 1;
    m_FoldingID = 2;
    autoCompleteNumber = 2;
    autocomplete=true;

    // initialize language
    m_language = NULL;

    // default font for all styles
    SetViewEOL (g_CommonPrefs.displayEOLEnable);
    SetIndentationGuides (g_CommonPrefs.indentGuideEnable);
    SetEdgeMode (g_CommonPrefs.longLineOnEnable ? wxSTC_EDGE_LINE: wxSTC_EDGE_NONE);
    SetViewWhiteSpace (g_CommonPrefs.whiteSpaceEnable ? wxSTC_WS_VISIBLEALWAYS: wxSTC_WS_INVISIBLE);
    SetOvertype (g_CommonPrefs.overTypeInitial);
    SetReadOnly (g_CommonPrefs.readOnlyInitial);
    SetWrapMode (g_CommonPrefs.wrapModeInitial ? wxSTC_WRAP_WORD: wxSTC_WRAP_NONE);
    wxFont font (10, wxDECORATIVE, wxNORMAL, wxNORMAL);
    StyleSetFont (wxSTC_STYLE_DEFAULT, font);
    StyleSetForeground (wxSTC_STYLE_DEFAULT, *wxBLACK);
    StyleSetBackground (wxSTC_STYLE_DEFAULT, *wxWHITE);
    StyleSetForeground (wxSTC_STYLE_LINENUMBER, wxColour (_T("DARK GREY")));
    StyleSetBackground (wxSTC_STYLE_LINENUMBER, *wxWHITE);
    StyleSetForeground(wxSTC_STYLE_INDENTGUIDE, wxColour (_T("DARK GREY")));

    m_languagename = DeterminePrefs(filename);
    InitializePrefs (m_languagename);
    
    // set visibility
    SetVisiblePolicy (wxSTC_VISIBLE_STRICT|wxSTC_VISIBLE_SLOP, 1);
    SetXCaretPolicy (wxSTC_CARET_EVEN|wxSTC_VISIBLE_STRICT|wxSTC_CARET_SLOP, 1);
    SetYCaretPolicy (wxSTC_CARET_EVEN|wxSTC_VISIBLE_STRICT|wxSTC_CARET_SLOP, 1);

    // markers
    MarkerDefine (wxSTC_MARKNUM_FOLDER,        wxSTC_MARK_BOXPLUS, _T("WHITE"), _T("BLACK"));
    MarkerDefine (wxSTC_MARKNUM_FOLDEROPEN,    wxSTC_MARK_BOXMINUS, _T("WHITE"), _T("BLACK"));
    MarkerDefine (wxSTC_MARKNUM_FOLDERSUB,     wxSTC_MARK_EMPTY,     _T("BLACK"), _T("BLACK"));
    MarkerDefine (wxSTC_MARKNUM_FOLDEREND,     wxSTC_MARK_DOTDOTDOT, _T("BLACK"), _T("WHITE"));
    MarkerDefine (wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN, _T("BLACK"), _T("WHITE"));
    MarkerDefine (wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY,     _T("BLACK"), _T("BLACK"));
    MarkerDefine (wxSTC_MARKNUM_FOLDERTAIL,    wxSTC_MARK_EMPTY,     _T("BLACK"), _T("BLACK"));

    MarkerDefine (24,    wxSTC_MARK_ARROW,     _T("BLACK"), _T("YELLOW"));

    // miscelaneous
    m_LineNrMargin = TextWidth (wxSTC_STYLE_LINENUMBER, _T("_999999"));
    m_FoldingMargin = 16;
    //CmdKeyClear (wxSTC_KEY_TAB, 0); // this is done by the menu accelerator key
    SetLayoutCache (wxSTC_CACHE_PAGE);

}

Edit::~Edit () {}

//----------------------------------------------------------------------------
// common event handlers
void Edit::OnSize( wxSizeEvent& event ) {
    int x = GetClientSize().x +
            (g_CommonPrefs.lineNumberEnable? m_LineNrMargin: 0) +
            (g_CommonPrefs.foldEnable? m_FoldingMargin: 0);
    if (x > 0) SetScrollWidth (x);
    event.Skip();
}

// edit event handlers
void Edit::OnEditRedo (wxCommandEvent &WXUNUSED(event)) {
    if (!CanRedo()) return;
    Redo ();
}

void Edit::OnEditUndo (wxCommandEvent &WXUNUSED(event)) {
    if (!CanUndo()) return;
    Undo ();
}

void Edit::OnEditClear (wxCommandEvent &WXUNUSED(event)) {
    if (GetReadOnly()) return;
    Clear ();
}

void Edit::OnEditCut (wxCommandEvent &WXUNUSED(event)) {
    if (GetReadOnly() || (GetSelectionEnd()-GetSelectionStart() <= 0)) return;
    Cut ();
}

void Edit::OnEditCopy (wxCommandEvent &WXUNUSED(event)) {
    if (GetSelectionEnd()-GetSelectionStart() <= 0) return;
    Copy ();
}

void Edit::OnEditPaste (wxCommandEvent &WXUNUSED(event)) {
    if (!CanPaste()) return;
    Paste ();
}

void Edit::OnFind (wxCommandEvent &WXUNUSED(event)) {
}

void Edit::OnFindNext (wxCommandEvent &WXUNUSED(event)) {
}

void Edit::OnReplace (wxCommandEvent &WXUNUSED(event)) {
}

void Edit::OnReplaceNext (wxCommandEvent &WXUNUSED(event)) {
}

void Edit::OnBraceMatch (wxCommandEvent &WXUNUSED(event)) {
    int min = GetCurrentPos ();
    int max = BraceMatch (min);
    if (max > (min+1)) {
        BraceHighlight (min+1, max);
        SetSelection (min+1, max);
    }else{
        BraceBadLight (min);
    }
}

void Edit::OnGoto (wxCommandEvent &WXUNUSED(event)) {
}

void Edit::OnEditIndentInc (wxCommandEvent &WXUNUSED(event)) {
    CmdKeyExecute (wxSTC_CMD_TAB);
}

void Edit::OnEditIndentRed (wxCommandEvent &WXUNUSED(event)) {
    CmdKeyExecute (wxSTC_CMD_BACKTAB);
}

void Edit::OnEditSelectAll (wxCommandEvent &WXUNUSED(event)) {
    SetSelection (0, GetTextLength ());
}

void Edit::OnEditSelectLine (wxCommandEvent &WXUNUSED(event)) {
    int lineStart = PositionFromLine (GetCurrentLine());
    int lineEnd = PositionFromLine (GetCurrentLine() + 1);
    SetSelection (lineStart, lineEnd);
}

void Edit::OnHilightLang (wxCommandEvent &event) {
    InitializePrefs (g_LanguagePrefs [event.GetId() - myID_HILIGHTFIRST].name);
}

void Edit::OnDisplayEOL (wxCommandEvent &WXUNUSED(event)) {
    SetViewEOL (!GetViewEOL());
}

void Edit::OnIndentGuide (wxCommandEvent &WXUNUSED(event)) {
    SetIndentationGuides (!GetIndentationGuides());
}

void Edit::OnLineNumber (wxCommandEvent &WXUNUSED(event)) {
    SetMarginWidth (m_LineNrID,
                    GetMarginWidth (m_LineNrID) == 0? m_LineNrMargin: 0);
}

void Edit::OnLongLineOn (wxCommandEvent &WXUNUSED(event)) {
    SetEdgeMode (GetEdgeMode() == 0? wxSTC_EDGE_LINE: wxSTC_EDGE_NONE);
}

void Edit::OnWhiteSpace (wxCommandEvent &WXUNUSED(event)) {
    SetViewWhiteSpace (GetViewWhiteSpace() == 0?
                       wxSTC_WS_VISIBLEALWAYS: wxSTC_WS_INVISIBLE);
}

void Edit::OnFoldToggle (wxCommandEvent &WXUNUSED(event)) {
    ToggleFold (GetFoldParent(GetCurrentLine()));
}

void Edit::OnSetOverType (wxCommandEvent &WXUNUSED(event)) {
    SetOvertype (!GetOvertype());
}

void Edit::OnSetReadOnly (wxCommandEvent &WXUNUSED(event)) {
    SetReadOnly (!GetReadOnly());
}

void Edit::OnWrapmodeOn (wxCommandEvent &WXUNUSED(event)) {
    SetWrapMode (GetWrapMode() == 0? wxSTC_WRAP_WORD: wxSTC_WRAP_NONE);
}

void Edit::ClearHotkeys() { m_hotkey.Clear(); }  // PL: Questa roba secondo me non serve più
void Edit::AddHotkey(wxString s) {   // PL: Idem con patate
    wxString s2;
    if ( s[1]==',' ) s2 = s;
    if ( s[2]==',' ) {
        int k = wxHexToDec(s.Left(2));
        s2.Printf(_T("%c,"),k);
        s2=s2+s.Mid(3);
    }
    m_hotkey.Add(s2); 
}

void Edit::OnAutocompOn (wxCommandEvent &WXUNUSED(event)) {
    autocomplete = (!autocomplete) && autocompletemain;
}

void Edit::OnHotkeysOn (wxCommandEvent &WXUNUSED(event)) {
    hotkeys = (!hotkeys) && hotkeysmain;
}

void Edit::OnUseCharset (wxCommandEvent &event) {
    int Nr;
    int charset = GetCodePage();
    switch (event.GetId()) {
        case myID_CHARSETANSI: {charset = wxSTC_CHARSET_ANSI; break;}
        case myID_CHARSETMAC: {charset = wxSTC_CHARSET_ANSI; break;}
    }
    for (Nr = 0; Nr < wxSTC_STYLE_LASTPREDEFINED; Nr++) {
        StyleSetCharacterSet (Nr, charset);
    }
    SetCodePage (charset);
}

void Edit::OnChangeCase (wxCommandEvent &event) {
    switch (event.GetId()) {
        case myID_CHANGELOWER: {
            CmdKeyExecute (wxSTC_CMD_LOWERCASE);
            break;
        }
        case myID_CHANGEUPPER: {
            CmdKeyExecute (wxSTC_CMD_UPPERCASE);
            break;
        }
    }
}

void Edit::OnConvertEOL (wxCommandEvent &event) {
    int eolMode = GetEOLMode();
    switch (event.GetId()) {
        case myID_CONVERTCR: { eolMode = wxSTC_EOL_CR; break;}
        case myID_CONVERTCRLF: { eolMode = wxSTC_EOL_CRLF; break;}
        case myID_CONVERTLF: { eolMode = wxSTC_EOL_LF; break;}
    }
    ConvertEOLs (eolMode);
    SetEOLMode (eolMode);
}

//! misc
void Edit::OnMarginClick (wxStyledTextEvent &event) {
    if (event.GetMargin() == 2) {
        int lineClick = LineFromPosition (event.GetPosition());
        int levelClick = GetFoldLevel (lineClick);
        if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0) {
            ToggleFold (lineClick);
        }
    }
}

bool Edit::HasWord(wxString word, wxString &wordlist)
{
	word = word.MakeLower();

	wxStringTokenizer tkz(wordlist.Lower(), _T(" "));
	while (tkz.HasMoreTokens())
	{	wxString token = tkz.GetNextToken();

		if (token.CmpNoCase(word)==0)
			continue;
		if (token.StartsWith(word))
			return true;
	}
	return false;
}

/* struct Hkeys {
	const int chr;
	const char *str;
}; */

// Evidentemente non sono mai contento di questa routine

void Edit::OnCharAdded (wxStyledTextEvent &event) {
    int pos = GetCurrentPos();
    int chr = (char) event.GetKey();
    //printf("%d*",chr);
    bool found = false;
    int i;
	wxString zstr;
	//Hkeys hotkeylist[] = { {-85, "Pippo"}, {-69, "Pluto"} };
	
	const char *hklist[] = {
		"-", "@!!", "-", "@LL", "-", "-", "-", "-", "-", "-", "-", "@<<", "-", "-", "-", "-",
		"-", "-", "-", "-", "-", "-", "-", "-", "-", "-", "-", "@>>", "@OE", "@oe", "-", "@??",
		"@`A", "@'A", "@^A", "@~A", "@:A", "@oA", "@AE", "@,C", "@`E", "@'E", "@^E", "@:E", "@`I", "@'I", "@^I", "@:I",
		"@ET", "@~N", "@`O", "@'O", "@^O", "@~O", "@:O", "-", "@\\O", "@`U", "@'U", "@^U", "@:U", "@'Y", "@Th", "@ss",
		"@`a", "@'a", "@^a", "@~a", "@:a", "@oa", "@ae", "@,c", "@`e", "@'e", "@^e", "@:e", "@`i", "@'i", "@^i", "@:i",
		"@et", "@~n", "@`o", "@'o", "@^o", "@~o", "@:o", "-", "@\\o", "@`u", "@'u", "@^u", "@:u", "@'y", "@th", "@:y"
	};

/*    Hkeys hotkeylist[] = {
		{-85, "@<<"}, {-69, "@>>"}, {-64, "@`A"}, {-56, "@`E"}, {-55, "@'E"}, {-52, "@`I"}, {-46, "@`O"}, {-39, "@`U"},	
		{-32, "@`a"}, {-24, "@`e"}, {-23, "@'e"}, {-20, "@`i"}, {-14, "@`o"}, {-7, "@`u"},
		{0," "}
    }; */
    
    if (hotkeys) {    
		i = -1;
		if (chr < 0 && chr >= -96) i = 96+chr;   // 256+chr-160;
		if (chr >= 65440 && chr < 65536) i = chr-65440;
		if (i>=0 && i<96) if (*hklist[i] != '@') i = -1;
		if (i>=0 && i<96) {
			zstr = wxString::FromUTF8(hklist[i]);
			found = true;
		}
		/*
		for (i=0;hotkeylist[i].chr;i++)  {
			if ((chr == hotkeylist[i].chr) || (chr == (65536 + hotkeylist[i].chr))) {
				zstr = wxString::FromUTF8(hotkeylist[i].str);
				found = true;
				break;
			}
		}
		*/

/*		PL: Esempio di come era fatto prima
        switch (chr) {
            case 65451: case -85:
                zstr=_T("@<<");
                found = true;
                break;
			[...]
        }
*/
		if (found) {
			SetTargetStart(pos - 2);
            SetTargetEnd(pos);
            ReplaceTarget(_T(""));
			AddText(zstr);
		}
    }
    // if (chr < 0) { wxString s; s.Printf("%d",chr); AddText(s); }        
//    if (hotkeys) {          
//        for (size_t i = 0; i < m_hotkey.GetCount(); i++) {
//            wxString s = m_hotkey[i];
//            printf(" %d ",s[0]);
//            if (s[0] == chr) {                
//                SetTargetStart(pos - 1);
//                SetTargetEnd(pos);
//                ReplaceTarget(_T(""));
//                AddText(s.Mid(2));
//            }
//        }
//    }
    int currentLine = GetCurrentLine();
    // Change this if support for mac files with \r is needed
    if (chr == '\n' || chr == '\r') {
    	// Update the main tree in the frame
        m_frame->RefreshTree();                
        int lineInd = 0;
        if (currentLine > 0) {
            lineInd = GetLineIndentation(currentLine - 1);
        }
        if (lineInd == 0) return;
        SetLineIndentation (currentLine, lineInd);
        GotoPos(PositionFromLine (currentLine) + lineInd);
    }
    
    // new automcpletion code
    if (!found){
        pos = GetCurrentPos();
        int start = WordStartPosition(pos, true);
        if (pos - start >= 1) {
            if (HasWord(GetTextRange(start, pos), m_wordlist)) {
                if (!AutoCompActive())
                    AutoCompShow(pos - start, m_wordlist);
            } else
                AutoCompCancel();
        }    
    }
    // new automcpletion code
    
    /*
    // SKILLO
    int style = GetStyleAt(pos);
    if (autocomplete && style != 5 && style != 8) {    // Valori trovati per tentativi
        int start = WordStartPosition(pos, true);
        if (pos - start >= autoCompleteNumber && !AutoCompActive())    // require x characters to show auto-complete
        {
    //        if (HasWord(GetTextRange(start, pos), m_wordlist))
                AutoCompShow(pos-start, m_wordlist);
        }
        else{
            AutoCompCancel();
        }
        // SKILLO
     * 
    }   */ 
}

//----------------------------------------------------------------------------
// private functions
wxString Edit::DeterminePrefs (const wxString &filename) {

    LanguageInfo const* curInfo;

    // determine language from filepatterns
    int languageNr;
    for (languageNr = 0; languageNr < g_LanguagePrefsSize; languageNr++) {
        curInfo = &g_LanguagePrefs [languageNr];
        wxString filepattern = curInfo->filepattern;
        filepattern.Lower();
        while (!filepattern.empty()) {
            wxString cur = filepattern.BeforeFirst (';');
            if ((cur == filename) ||
                (cur == (filename.BeforeLast ('.') + _T(".*"))) ||
                (cur == (_T("*.") + filename.AfterLast ('.')))) {
                return curInfo->name;
            }
            filepattern = filepattern.AfterFirst (';');
        }
    }
    return wxEmptyString;
}

bool Edit::InitializePrefs (const wxString &name) {

    // initialize styles
    StyleClearAll();
    LanguageInfo const* curInfo = NULL;

    // determine language
    bool found = false;
    int languageNr;
    for (languageNr = 0; languageNr < g_LanguagePrefsSize; languageNr++) {
        curInfo = &g_LanguagePrefs [languageNr];
        if (curInfo->name == name) {
            found = true;
            break;
        }
    }
    if (!found) return false;

    // set lexer and language
    SetLexer (curInfo->lexer);
    m_language = curInfo;

    // set margin for line numbers
    SetMarginType (m_LineNrID, wxSTC_MARGIN_NUMBER);
    StyleSetForeground (wxSTC_STYLE_LINENUMBER, wxColour (_T("BLACK")));
    SetMarginWidth (m_LineNrID, 0); // start out not visible

    // default fonts for all styles!
    int Nr;
    for (Nr = 0; Nr < wxSTC_STYLE_LASTPREDEFINED; Nr++) {
        wxFont font (10, wxTELETYPE, wxNORMAL, wxNORMAL);
        StyleSetFont (Nr, font);
    }

    // set common styles
    StyleSetForeground (wxSTC_STYLE_DEFAULT, wxColour (_T("DARK GREY")));
    StyleSetForeground (wxSTC_STYLE_INDENTGUIDE, wxColour (_T("DARK GREY")));
    

    // initialize settings
    if (g_CommonPrefs.syntaxEnable) {
        int keywordnr = 0;
        for (Nr = 0; Nr < STYLE_TYPES_COUNT; Nr++) {
            if (curInfo->styles[Nr].type == -1) continue;
            const StyleInfo &curType = g_StylePrefs [curInfo->styles[Nr].type];
            wxFont font (curType.fontsize, wxTELETYPE, wxNORMAL, wxNORMAL, false,
                         curType.fontname);
            StyleSetFont (Nr, font);
            if (curType.foreground) {
                StyleSetForeground (Nr, wxColour (curType.foreground));
            }
            if (curType.background) {
                StyleSetBackground (Nr, wxColour (curType.background));
            }
            StyleSetBold (Nr, (curType.fontstyle & mySTC_STYLE_BOLD) > 0);
            StyleSetItalic (Nr, (curType.fontstyle & mySTC_STYLE_ITALIC) > 0);
            StyleSetUnderline (Nr, (curType.fontstyle & mySTC_STYLE_UNDERL) > 0);
            StyleSetVisible (Nr, (curType.fontstyle & mySTC_STYLE_HIDDEN) == 0);
            StyleSetCase (Nr, curType.lettercase);
            const wxChar *pwords = curInfo->styles[Nr].words;
            if (pwords) {
                SetKeyWords (keywordnr, pwords);
                keywordnr += 1;
            }
        }
    }

    // set margin as unused
    SetMarginType (m_DividerID, wxSTC_MARGIN_SYMBOL);
    SetMarginWidth (m_DividerID, 0);
    SetMarginSensitive (m_DividerID, false);

    // folding
    SetMarginType (m_FoldingID, wxSTC_MARGIN_SYMBOL);
//    SetMarginMask (m_FoldingID, wxSTC_MASK_FOLDERS);
    SetMarginMask (m_FoldingID, -1);
    StyleSetBackground (m_FoldingID, *wxWHITE);
    SetMarginWidth (m_FoldingID, 0);
    SetMarginSensitive (m_FoldingID, false);
    if (g_CommonPrefs.foldEnable) {
        SetMarginWidth (m_FoldingID, curInfo->folds != 0? m_FoldingMargin: 0);
        SetMarginSensitive (m_FoldingID, curInfo->folds != 0);
        SetProperty (_T("fold"), curInfo->folds != 0? _T("1"): _T("0"));
        SetProperty (_T("fold.comment"),
                     (curInfo->folds & mySTC_FOLD_COMMENT) > 0? _T("1"): _T("0"));
        SetProperty (_T("fold.compact"),
                     (curInfo->folds & mySTC_FOLD_COMPACT) > 0? _T("1"): _T("0"));
        SetProperty (_T("fold.preprocessor"),
                     (curInfo->folds & mySTC_FOLD_PREPROC) > 0? _T("1"): _T("0"));
        SetProperty (_T("fold.html"),
                     (curInfo->folds & mySTC_FOLD_HTML) > 0? _T("1"): _T("0"));
        SetProperty (_T("fold.html.preprocessor"),
                     (curInfo->folds & mySTC_FOLD_HTMLPREP) > 0? _T("1"): _T("0"));
        SetProperty (_T("fold.comment.python"),
                     (curInfo->folds & mySTC_FOLD_COMMENTPY) > 0? _T("1"): _T("0"));
        SetProperty (_T("fold.quotes.python"),
                     (curInfo->folds & mySTC_FOLD_QUOTESPY) > 0? _T("1"): _T("0"));
    }
    SetFoldFlags (wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED |
                  wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);

    // set spaces and indention
    SetTabWidth (2);
    SetUseTabs (false);
    SetTabIndents (false);
    SetBackSpaceUnIndents (true);
    SetIndent (g_CommonPrefs.indentEnable? 2: 0);

    // others
    SetViewEOL (g_CommonPrefs.displayEOLEnable);
    SetIndentationGuides (g_CommonPrefs.indentGuideEnable);
    SetEdgeColumn (80);
    SetEdgeMode (g_CommonPrefs.longLineOnEnable? wxSTC_EDGE_LINE: wxSTC_EDGE_NONE);
    SetViewWhiteSpace (g_CommonPrefs.whiteSpaceEnable?
                       wxSTC_WS_VISIBLEALWAYS: wxSTC_WS_INVISIBLE);
    SetOvertype (g_CommonPrefs.overTypeInitial);
    SetReadOnly (g_CommonPrefs.readOnlyInitial);
    SetWrapMode (g_CommonPrefs.wrapModeInitial?
                 wxSTC_WRAP_WORD: wxSTC_WRAP_NONE);

    return true;
}

bool Edit::LoadFile ()
{
#if wxUSE_FILEDLG
    // get filname
    if (!m_filename) {
        wxFileDialog dlg (this, _T("Open file"), wxEmptyString, wxEmptyString,
                          _T("Any file (*)|*"), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);
        if (dlg.ShowModal() != wxID_OK) return false;
        m_filename = dlg.GetPath();
    }

    // load file
    return LoadFile (m_filename);
#else
    return false;
#endif // wxUSE_FILEDLG
}


bool Edit::LoadFile(const wxString& filename)
{
	if (!filename.empty()) m_filename = filename;
	ClearAll ();
    bool success = false;
    wxFile file(filename, wxFile::read);

    if (file.IsOpened())
    {
        wxString contents;
        // get the file size (assume it is not huge file...)
        ssize_t len = (ssize_t)file.Length();

        if (len > 0)
        {
            wxMemoryBuffer buffer(len+1);
            success = (file.Read(buffer.GetData(), len) == len);
            if (success) {
                ((char*)buffer.GetData())[len] = 0;
                contents = wxString(buffer, wxConvISO8859_1, len);
            }
        }
        else
        {
            if (len == 0)
                success = true;  // empty file is ok
            else
                success = false; // len == wxInvalidOffset
        }

        if (success)
        {
            SetText(contents);
            EmptyUndoBuffer();
            SetSavePoint();
        }
    }
 	EmptyUndoBuffer();

    // determine lexer language
    wxFileName fname (m_filename);
    InitializePrefs (DeterminePrefs (fname.GetFullName()));
    return success;
}





bool Edit::SaveFile ()
{
#if wxUSE_FILEDLG
    // return if no change
    if (!Modified()) return true;

    // get filname
    if (!m_filename) {
        wxFileDialog dlg (this, _T("Save file"), wxEmptyString, wxEmptyString, _T("Any file (*)|*"),
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if (dlg.ShowModal() != wxID_OK) return false;
        m_filename = dlg.GetPath();
    }

    // save file
    return SaveFile (m_filename);
#else
    return false;
#endif // wxUSE_FILEDLG
}

bool Edit::SaveFile(const wxString& filename)
{
	if (!Modified()) return true;
    wxFile file(filename, wxFile::write);

    if (!file.IsOpened())
        return false;

    bool success = file.Write(GetText(), wxConvISO8859_1);

    if (success)
        SetSavePoint();

    return success;
}

bool Edit::Modified () {

    // return modified state
    return (GetModify() && !GetReadOnly());
}

//----------------------------------------------------------------------------
// EditProperties
//----------------------------------------------------------------------------

EditProperties::EditProperties (Edit *edit,
                                long style)
        : wxDialog (edit, wxID_ANY, wxEmptyString,
                    wxDefaultPosition, wxDefaultSize,
                    style | wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER) {

    // sets the application title
    SetTitle (_("Properties"));
    wxString text;

    // fullname
    wxBoxSizer *fullname = new wxBoxSizer (wxHORIZONTAL);
    fullname->Add (10, 0);
    fullname->Add (new wxStaticText (this, wxID_ANY, _("Full filename"),
                                     wxDefaultPosition, wxSize(80, wxDefaultCoord)),
                   0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
    fullname->Add (new wxStaticText (this, wxID_ANY, edit->GetFilename()),
                   0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);

    // text info
    wxGridSizer *textinfo = new wxGridSizer (4, 0, 2);
    textinfo->Add (new wxStaticText (this, wxID_ANY, _("Language"),
                                     wxDefaultPosition, wxSize(80, wxDefaultCoord)),
                   0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT, 4);
    textinfo->Add (new wxStaticText (this, wxID_ANY, edit->m_language->name),
                   0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxRIGHT, 4);
    textinfo->Add (new wxStaticText (this, wxID_ANY, _("Lexer-ID: "),
                                     wxDefaultPosition, wxSize(80, wxDefaultCoord)),
                   0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT, 4);
    text = wxString::Format (_T("%d"), edit->GetLexer());
    textinfo->Add (new wxStaticText (this, wxID_ANY, text),
                   0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxRIGHT, 4);
    wxString EOLtype = wxEmptyString;
    switch (edit->GetEOLMode()) {
        case wxSTC_EOL_CR: {EOLtype = _T("CR (Unix)"); break; }
        case wxSTC_EOL_CRLF: {EOLtype = _T("CRLF (Windows)"); break; }
        case wxSTC_EOL_LF: {EOLtype = _T("CR (Macintosh)"); break; }
    }
    textinfo->Add (new wxStaticText (this, wxID_ANY, _("Line endings"),
                                     wxDefaultPosition, wxSize(80, wxDefaultCoord)),
                   0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT, 4);
    textinfo->Add (new wxStaticText (this, wxID_ANY, EOLtype),
                   0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxRIGHT, 4);

    // text info box
    wxStaticBoxSizer *textinfos = new wxStaticBoxSizer (
                     new wxStaticBox (this, wxID_ANY, _("Informations")),
                     wxVERTICAL);
    textinfos->Add (textinfo, 0, wxEXPAND);
    textinfos->Add (0, 6);

    // statistic
    wxGridSizer *statistic = new wxGridSizer (4, 0, 2);
    statistic->Add (new wxStaticText (this, wxID_ANY, _("Total lines"),
                                     wxDefaultPosition, wxSize(80, wxDefaultCoord)),
                    0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT, 4);
    text = wxString::Format (_T("%d"), edit->GetLineCount());
    statistic->Add (new wxStaticText (this, wxID_ANY, text),
                    0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxRIGHT, 4);
    statistic->Add (new wxStaticText (this, wxID_ANY, _("Total chars"),
                                     wxDefaultPosition, wxSize(80, wxDefaultCoord)),
                    0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT, 4);
    text = wxString::Format (_T("%d"), edit->GetTextLength());
    statistic->Add (new wxStaticText (this, wxID_ANY, text),
                    0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxRIGHT, 4);
    statistic->Add (new wxStaticText (this, wxID_ANY, _("Current line"),
                                     wxDefaultPosition, wxSize(80, wxDefaultCoord)),
                    0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT, 4);
    text = wxString::Format (_T("%d"), edit->GetCurrentLine());
    statistic->Add (new wxStaticText (this, wxID_ANY, text),
                    0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxRIGHT, 4);
    statistic->Add (new wxStaticText (this, wxID_ANY, _("Current pos"),
                                     wxDefaultPosition, wxSize(80, wxDefaultCoord)),
                    0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT, 4);
    text = wxString::Format (_T("%d"), edit->GetCurrentPos());
    statistic->Add (new wxStaticText (this, wxID_ANY, text),
                    0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxRIGHT, 4);

    // char/line statistics
    wxStaticBoxSizer *statistics = new wxStaticBoxSizer (
                     new wxStaticBox (this, wxID_ANY, _("Statistics")),
                     wxVERTICAL);
    statistics->Add (statistic, 0, wxEXPAND);
    statistics->Add (0, 6);

    // total pane
    wxBoxSizer *totalpane = new wxBoxSizer (wxVERTICAL);
    totalpane->Add (fullname, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
    totalpane->Add (0, 6);
    totalpane->Add (textinfos, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
    totalpane->Add (0, 10);
    totalpane->Add (statistics, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
    totalpane->Add (0, 6);
    wxButton *okButton = new wxButton (this, wxID_OK, _("OK"));
    okButton->SetDefault();
    totalpane->Add (okButton, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizerAndFit (totalpane);

    ShowModal();
}

#if wxUSE_PRINTING_ARCHITECTURE

//----------------------------------------------------------------------------
// EditPrint
//----------------------------------------------------------------------------

EditPrint::EditPrint (Edit *edit, wxChar *title)
              : wxPrintout(title) {
    m_edit = edit;
    m_printed = 0;

}

bool EditPrint::OnPrintPage (int page) {

    wxDC *dc = GetDC();
    if (!dc) return false;

    // scale DC
    PrintScaling (dc);

    // print page
    if (page == 1) m_printed = 0;
    m_printed = m_edit->FormatRange (1, m_printed, m_edit->GetLength(),
                                     dc, dc, m_printRect, m_pageRect);

    return true;
}

bool EditPrint::OnBeginDocument (int startPage, int endPage) {

    if (!wxPrintout::OnBeginDocument (startPage, endPage)) {
        return false;
    }

    return true;
}

bool EditPrint::HasPage (int WXUNUSED(page)) {

    return (m_printed < m_edit->GetLength());
}

bool EditPrint::PrintScaling (wxDC *dc){

    // check for dc, return if none
    if (!dc) return false;

    // get printer and screen sizing values
    wxSize ppiScr;
    GetPPIScreen (&ppiScr.x, &ppiScr.y);
    if (ppiScr.x == 0) { // most possible guess 96 dpi
        ppiScr.x = 96;
        ppiScr.y = 96;
    }
    wxSize ppiPrt;
    GetPPIPrinter (&ppiPrt.x, &ppiPrt.y);
    if (ppiPrt.x == 0) { // scaling factor to 1
        ppiPrt.x = ppiScr.x;
        ppiPrt.y = ppiScr.y;
    }
    wxSize dcSize = dc->GetSize();
    wxSize pageSize;
    GetPageSizePixels (&pageSize.x, &pageSize.y);

    // set user scale
    float scale_x = (float)(ppiPrt.x * dcSize.x) /
                    (float)(ppiScr.x * pageSize.x);
    float scale_y = (float)(ppiPrt.y * dcSize.y) /
                    (float)(ppiScr.y * pageSize.y);
    dc->SetUserScale (scale_x, scale_y);

    return true;
}

#endif // wxUSE_PRINTING_ARCHITECTURE
