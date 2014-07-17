//////////////////////////////////////////////////////////////////////////////
// File:        contrib/samples/stc/prefs.cpp
// Purpose:     STC test Preferences initialization
// Maintainer:  Wyo
// Created:     2003-09-01
// Copyright:   (c) wxGuide
// Licence:     wxWindows licence
// Modified by: silver.slade@tiscali.it
//////////////////////////////////////////////////////////////////////////////

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

//! wxWidgets/contrib headers

//! application headers
#include "defsext.h"     // Additional definitions
#include "prefs.h"       // Preferences

#define FONTSIZE 9

//============================================================================
// declarations
//============================================================================

//----------------------------------------------------------------------------
//! language types
const CommonInfo g_CommonPrefs = {
    // editor functionality prefs
    true,  // syntaxEnable
    true,  // foldEnable
    true,  // indentEnable
    // display defaults prefs
    false, // overTypeInitial
    false, // readOnlyInitial
    false,  // wrapModeInitial
    false, // displayEOLEnable
    false, // IndentGuideEnable
    true,  // lineNumberEnable
    true, // longLineOnEnable
    false, // whiteSpaceEnable
};

//----------------------------------------------------------------------------
// keywordlists
// C++
const wxChar* InformWordlist1 =
    _T("");
const wxChar* InformWordlist2 =
    _T("");
const wxChar* InformWordlist3 =
    _T("");




//----------------------------------------------------------------------------
//! languages
const LanguageInfo g_LanguagePrefs [] = {
    // C++
    {(wxChar*)_T("INFORM"),
     (wxChar*)_T("*.h;*.inf;*.INF;*.H"),
     //wxSTC_LEX_OCTAVE,
     wxSTC_LEX_MATLAB,
     {{mySTC_TYPE_DEFAULT, NULL},
      {mySTC_TYPE_COMMENT, NULL},
      {mySTC_TYPE_COMMENT_LINE, NULL},
      {mySTC_TYPE_COMMENT_DOC, NULL},
      {mySTC_TYPE_NUMBER, NULL},
      {mySTC_TYPE_WORD1, InformWordlist1}, // KEYWORDS
      {mySTC_TYPE_STRING, NULL},
      {mySTC_TYPE_CHARACTER, NULL},
      {mySTC_TYPE_UUID, NULL},
      {mySTC_TYPE_PREPROCESSOR, NULL},
      {mySTC_TYPE_OPERATOR, NULL},
      {mySTC_TYPE_IDENTIFIER, NULL},
      {mySTC_TYPE_STRING_EOL, NULL},
      {mySTC_TYPE_DEFAULT, NULL}, // VERBATIM
      {mySTC_TYPE_REGEX, NULL},
      {mySTC_TYPE_COMMENT_SPECIAL, NULL}, // DOXY
      {mySTC_TYPE_WORD2, InformWordlist2}, // EXTRA WORDS
      {mySTC_TYPE_WORD3, InformWordlist3}, // DOXY KEYWORDS
      {mySTC_TYPE_ERROR, NULL}, // KEYWORDS ERROR
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL}},
     mySTC_FOLD_COMMENT | mySTC_FOLD_COMPACT | mySTC_FOLD_PREPROC}    
    };

const int g_LanguagePrefsSize = WXSIZEOF(g_LanguagePrefs);

//----------------------------------------------------------------------------
//! style types
const StyleInfo g_StylePrefs [] = {

    // mySTC_TYPE_DEFAULT
    {(wxChar*)_T("Default"),
     (wxChar*)_T("BLACK"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, 0, 0},

// Stringhe con '
    // mySTC_TYPE_WORD1
    {(wxChar*)_T("Keyword1"),
     (wxChar*)_T("ORANGE"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, mySTC_STYLE_BOLD, 0},

    // mySTC_TYPE_WORD2
    {(wxChar*)_T("Keyword2"),
     (wxChar*)_T("DARK BLUE"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_WORD3
    {(wxChar*)_T("Keyword3"),
     (wxChar*)_T("CORNFLOWER BLUE"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_WORD4
    {(wxChar*)_T("Keyword4"),
     (wxChar*)_T("CYAN"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_WORD5
    {(wxChar*)_T("Keyword5"),
     (wxChar*)_T("DARK GREY"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_WORD6
    {(wxChar*)_T("Keyword6"),
     (wxChar*)_T("GREY"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, 0, 0},

// COMMENTI
    // mySTC_TYPE_COMMENT
    {(wxChar*)_T("Comment"),
     (wxChar*)_T("GREY"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, mySTC_STYLE_BOLD|mySTC_STYLE_ITALIC, 0},

    // mySTC_TYPE_COMMENT_DOC
    {(wxChar*)_T("Comment (Doc)"),
     (wxChar*)_T("BLACK"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, mySTC_STYLE_ITALIC, 0},

    // mySTC_TYPE_COMMENT_LINE
    {(wxChar*)_T("Comment line"),
     (wxChar*)_T("GREY"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, mySTC_STYLE_BOLD|mySTC_STYLE_ITALIC, 0},

    // mySTC_TYPE_COMMENT_SPECIAL
    {(wxChar*)_T("Special comment"),
     (wxChar*)_T("BLACK"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, mySTC_STYLE_ITALIC, 0},

// testo normale
    // mySTC_TYPE_CHARACTER
    {(wxChar*)_T("Character"),
     (wxChar*)_T("BLACK"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_CHARACTER_EOL
    {(wxChar*)_T("Character (EOL)"),
     (wxChar*)_T("KHAKI"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_STRING
    {(wxChar*)_T("String"),
     (wxChar*)_T("BROWN"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_STRING_EOL
    {(wxChar*)_T("String (EOL)"),
     (wxChar*)_T("BROWN"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_DELIMITER
    {(wxChar*)_T("Delimiter"),
     (wxChar*)_T("ORANGE"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_PUNCTUATION
    {(wxChar*)_T("Punctuation"),
     (wxChar*)_T("ORANGE"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_OPERATOR
    {(wxChar*)_T("Operator"),
     (wxChar*)_T("BLACK"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, mySTC_STYLE_BOLD, 0},

    // mySTC_TYPE_BRACE
    {(wxChar*)_T("Label"),
     (wxChar*)_T("VIOLET"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_COMMAND
    {(wxChar*)_T("Command"),
     (wxChar*)_T("BLUE"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_IDENTIFIER
    {(wxChar*)_T("Identifier"),
     (wxChar*)_T("BLACK"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_LABEL
    {(wxChar*)_T("Label"),
     (wxChar*)_T("VIOLET"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, 0, 0},

// WORDLIST 1
    // mySTC_TYPE_NUMBER
    {(wxChar*)_T("Number"),
     (wxChar*)_T("NAVY"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_PARAMETER
    {(wxChar*)_T("Parameter"),
     (wxChar*)_T("VIOLET"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, mySTC_STYLE_ITALIC, 0},

    // mySTC_TYPE_REGEX
    {(wxChar*)_T("Regular expression"),
     (wxChar*)_T("ORCHID"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, 0, 0},

// Stringhe
    // mySTC_TYPE_UUID
    {(wxChar*)_T("UUID"),
     (wxChar*)_T("FOREST GREEN"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, mySTC_STYLE_BOLD, 0},

    // mySTC_TYPE_VALUE
    {(wxChar*)_T("Value"),
     (wxChar*)_T("ORCHID"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, mySTC_STYLE_ITALIC, 0},

    // mySTC_TYPE_PREPROCESSOR
    {(wxChar*)_T("Preprocessor"),
     (wxChar*)_T("GREY"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_SCRIPT
    {(wxChar*)_T("Script"),
     (wxChar*)_T("DARK GREY"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_ERROR
    {(wxChar*)_T("Error"),
     (wxChar*)_T("RED"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_UNDEFINED
    {(wxChar*)_T("Undefined"),
     (wxChar*)_T("ORANGE"), (wxChar*)_T("WHITE"),
     (wxChar*)_T(""), FONTSIZE, 0, 0}

    };

const int g_StylePrefsSize = WXSIZEOF(g_StylePrefs);
