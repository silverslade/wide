//////////////////////////////////////////////////////////////////////////////
// File:        main.cpp
// Purpose:     Wide Inform 6 Editor
// Author:      schillacia@users.berlios.de
// License:     GNU GPL
//////////////////////////////////////////////////////////////////////////////
  #include <wx/wx.h>
  #include "wx/aui/aui.h"
  #include "wx/treectrl.h"  
  #include "wx/artprov.h"
  #include "wx/menu.h"
  #include "wx/stc/stc.h"  
  #include "wx/aboutdlg.h" 
  #include "wx/filedlg.h"  
  #include "wx/toolbar.h"  
  #include "wx/imaglist.h"
  #include "wx/regex.h"    
  #include "wx/fileconf.h" 
  #include "wx/generic/numdlgg.h" 

  #include "images/fileopen.xpm"
  #include "images/filesave.xpm"
  #include "images/filesaveall.xpm" //PL
  #include "images/compilezcode.xpm"
  #include "images/compileulx.xpm"
  #include "images/exezcode.xpm"
  #include "images/exeglulx.xpm"
  #include "images/undo.xpm"
  #include "images/redo.xpm"
  #include "images/find.xpm"
  #include "images/findback.xpm" //PL
  #include "images/findobject.xpm" //PL
  #include "images/findobjectg.xpm" //PL
  #include "images/quit.xpm"
  #include "images/help.xpm"
  #include "images/copy.xpm"
  #include "images/cut.xpm"
  #include "images/paste.xpm"
  #include "images/new.xpm"
  #include "images/addbookm.xpm"
  #include "images/forward.xpm"
  #include "images/back.xpm"

  #include "defsext.h"     // Additional definitions
  #include "prefs.h"       // Prefs      
  #include "mytreeitemdata.h"
  #include "edit.h"
  #include "myframe.h"

  wxFileConfig *pConfig;       

  // our normal wxApp-derived class, as usual
 class MyApp : public wxApp {
 public:
 
   bool OnInit()
   {
     // Load config file
     pConfig = new wxFileConfig(
        _T(NOMEAPPLICAZIONE),
        _T(NOMEAPPLICAZIONE),
        _T(CONFIG_FILE),
        _T(""),
        wxCONFIG_USE_RELATIVE_PATH,
        wxConvISO8859_1);
     wxConfigBase::Set(pConfig);     
     pConfig->SetPath(_T("/"));
        
     // Window position and dimension
     int width = pConfig->Read(_T("WINDOW_WIDTH"), 1l);
     int height = pConfig->Read(_T("WINDOW_HEIGHT"), 1l);;
     wxPoint point = wxPoint(pConfig->Read(_T("WINDOW_LOCATION_X"), 1l),
                    pConfig->Read(_T("WINDOW_LOCATION_Y"), 1l));
        
     wxFrame* frame = new MyFrame(NULL,point,width,height);
     SetTopWindow(frame);
     frame->Show();
     
     return true;                    
   }
 };
 
 
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    // FILE MENU
    EVT_CLOSE (MyFrame::OnClose)
    EVT_MENU(MyFrame::ID_LoadFile,      MyFrame::OnLoadFile)
    EVT_MENU(MyFrame::ID_Exit,          MyFrame::OnExit)
    EVT_MENU(MyFrame::ID_About,         MyFrame::OnAbout)
    EVT_MENU(MyFrame::ID_NewFile,       MyFrame::OnNewFile)
    EVT_MENU(MyFrame::ID_NextPage,      MyFrame::OnNextPage)
    EVT_MENU(MyFrame::ID_PreviousPage,  MyFrame::OnPreviousPage)
    
    // MENU EDIT
    EVT_MENU (wxID_CUT,                MyFrame::OnSingleEdit)
    EVT_MENU (wxID_COPY,               MyFrame::OnSingleEdit)
    EVT_MENU (wxID_PASTE,              MyFrame::OnSingleEdit)
    
    EVT_MENU (MyFrame::ID_Comment,     MyFrame::OnComment)    
    EVT_MENU (MyFrame::ID_Uncomment,   MyFrame::OnUncomment)        
    
    // Search
    EVT_MENU(MyFrame::ID_GotoLine,     MyFrame::GotoLine)    
    EVT_MENU(MyFrame::ID_Find,         MyFrame::Search)
    EVT_MENU(MyFrame::ID_FindBack,     MyFrame::SearchBack)    //PL
    EVT_MENU(MyFrame::ID_FindObjectLocal,   MyFrame::SearchObjectLocal)    //PL
    EVT_MENU(MyFrame::ID_FindObjectGlobal,  MyFrame::SearchObjectGlobal)    //PL
    EVT_MENU(MyFrame::ID_NextMarker,   MyFrame::NextMarker)
    EVT_MENU(MyFrame::ID_ToggleMarker, MyFrame::ToggleMarker)  
    EVT_MENU(MyFrame::ID_ResetMarkers, MyFrame::ResetMarkers)  
    EVT_MENU(MyFrame::ID_FindReplace,  MyFrame::FindReplace)    //PL    

    // Project Menu
    EVT_MENU (ID_OpenProject,            MyFrame::OnOpenProject)
    EVT_MENU (ID_SaveProject,            MyFrame::OnSaveProject)
    EVT_MENU (ID_CloseProject,           MyFrame::OnCloseProject)        
    
    // ZCode Menu    
    EVT_MENU (ID_Compile,            MyFrame::OnCompile)
    EVT_MENU (ID_RunZcode,           MyFrame::OnRunZcode)    
    EVT_MENU (ID_ZcodeVersion5,      MyFrame::OnZcodeVersion)    
    EVT_MENU (ID_ZcodeVersion8,      MyFrame::OnZcodeVersion)
    EVT_MENU (ID_CreateZBlb,         MyFrame::OnCreateZBlb)
    EVT_MENU (ID_RunZBlb,            MyFrame::OnRunZBlb)         
    
    // Glulx Menu
    EVT_MENU (ID_CompileUlx,         MyFrame::OnCompileUlx)
    EVT_MENU (ID_RunUlx,             MyFrame::OnRunUlx)
    EVT_MENU (ID_RunBlb,             MyFrame::OnRunBlb)    
    EVT_MENU (ID_CreateBlb,          MyFrame::OnCreateBlb)    
    EVT_MENU (ID_CreateRes,          MyFrame::OnCreateRes)        
    EVT_MENU (ID_MakeAllBlb,         MyFrame::OnMakeAllBlb)
        
        
    // SYNTAX
    EVT_MENU (ID_UseDirectives,      MyFrame::OnSyntax)
    EVT_MENU (ID_UseStatements,      MyFrame::OnSyntax)
    EVT_MENU (ID_UseOtherkeywords,   MyFrame::OnSyntax)
    EVT_MENU (ID_UseInformate,       MyFrame::OnSyntax)
    EVT_MENU (ID_UseCustom1,         MyFrame::OnSyntax)
    EVT_MENU (ID_UseCustom2,         MyFrame::OnSyntax)
    EVT_MENU (ID_UseCustom3,         MyFrame::OnSyntax)

    // Object Tree    
    EVT_MENU (ID_RefreshTree,        MyFrame::OnRefreshTree)    
    EVT_MENU (ID_ShowObject,         MyFrame::OnOptions)
    EVT_MENU (ID_ShowProject,        MyFrame::OnOptions)
    EVT_MENU (ID_ShowGlobal,         MyFrame::OnOptions)
    EVT_MENU (ID_ShowConstant,       MyFrame::OnOptions)
    EVT_MENU (ID_ShowFunction,       MyFrame::OnOptions)
    EVT_MENU (ID_ShowClass,          MyFrame::OnOptions)
    EVT_MENU (ID_ShowInclude,        MyFrame::OnOptions)
    EVT_MENU (ID_ShowVerb,           MyFrame::OnOptions)
    EVT_MENU (ID_ExpandAllAlways,    MyFrame::OnOptions)
    
    // Eventi di STC
    EVT_MENU (wxID_FIND,             MyFrame::OnSingleEdit)
    EVT_MENU (myID_INDENTINC,        MyFrame::OnSingleEdit)
    EVT_MENU (myID_INDENTRED,        MyFrame::OnSingleEdit)    
    EVT_MENU (wxID_UNDO,             MyFrame::OnSingleEdit)
    EVT_MENU (wxID_REDO,             MyFrame::OnSingleEdit)
    EVT_MENU (ID_Save_File,          MyFrame::OnSaveFile)
    EVT_MENU (ID_Save_All,           MyFrame::OnSaveAll)      //PL

    // Opzioni
    EVT_MENU (myID_LINENUMBER,       MyFrame::OnEdit)
    EVT_MENU (myID_WRAPMODEON,       MyFrame::OnEdit) 
    EVT_MENU (myID_AUTOCOMPON,       MyFrame::OnEdit) 
    EVT_MENU (myID_HOTKEYSON,        MyFrame::OnEdit)
    EVT_MENU (myID_ENGLISH,          MyFrame::OnLanguage)    
    EVT_MENU (myID_ITALIAN,          MyFrame::OnLanguage)    
    EVT_MENU (myID_GERMAN,           MyFrame::OnLanguage)    
    EVT_MENU (myID_FRENCH,           MyFrame::OnLanguage)    
    EVT_MENU (myID_SPANISH,          MyFrame::OnLanguage)    

    


    // Eventi sull'oggetto TREE
    EVT_TREE_SEL_CHANGED(wxID_ANY, MyFrame::OnSelChanged)
    
    // Eventi sulla console
    EVT_STC_DOUBLECLICK(wxID_ANY, MyFrame::OnConsoleClicked)

    // Eventi sull'oggetto notebook
    EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, MyFrame::OnNotebookPageClose)
    EVT_AUINOTEBOOK_PAGE_CHANGED(wxID_ANY, MyFrame::OnNotebookPageChanged)
    
    // DOC MENU
    EVT_MENU (ID_Doc1,           MyFrame::OnViewDoc)        
    EVT_MENU (ID_Doc2,           MyFrame::OnViewDoc)        
    EVT_MENU (ID_Doc3,           MyFrame::OnViewDoc)                
    EVT_MENU (ID_Doc4,           MyFrame::OnViewDoc)                
    EVT_MENU (ID_Doc5,           MyFrame::OnViewDoc) 
    EVT_MENU (ID_Doc6,           MyFrame::OnViewDoc)
    EVT_MENU (ID_Doc7,           MyFrame::OnViewDoc)
    EVT_MENU (ID_Doc8,           MyFrame::OnViewDoc)    

END_EVENT_TABLE()

// Funzioni di classe ---------------------------------------------------------

// MENU EDIT: UNCOMMENT
void MyFrame::OnUncomment(wxCommandEvent& WXUNUSED(event)){
    if (auinotebook->GetPageCount()==0) return;
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());

    int start = e->GetSelectionStart();
    int end   = e->GetSelectionEnd();    
    int firstline = e->LineFromPosition(start);
    int lastline = e->LineFromPosition(end);
    for (int i = firstline; i <= lastline; i++)  {
        int linestart = e->PositionFromLine(i);
        int lineend = e->GetLineEndPosition(i);
        wxString tline = e->GetTextRange(linestart, lineend);
        for (size_t j = 0; j < tline.Length(); j++) {
            if (tline[j] == '!') { tline.Remove(j,1); end--; break; }
            if (tline[j] != ' ' && tline[j] != '\t') break;
        }
        e->SetTargetStart(linestart);
        e->SetTargetEnd(lineend);
        e->ReplaceTarget(tline);
    }    
    e->SetSelection(start, end);
}

// MENU EDIT: COMMENT
void MyFrame::OnComment(wxCommandEvent& WXUNUSED(event)){
    if (auinotebook->GetPageCount()==0) return;
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    
    int start = e->GetSelectionStart();
    int end   = e->GetSelectionEnd();
    int firstline = e->LineFromPosition(start);
    int lastline = e->LineFromPosition(end);
    for (int i = firstline; i <= lastline; i++) e->InsertText(e->PositionFromLine(i), _T("!"));
    end = end + lastline - firstline + 1;
    e->SetSelection(start, end);
}

// MENU FILE ****************
void MyFrame::OnNewFile(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog* fd = new wxFileDialog(this, _T("New File"),_T(""),_T(""),_T("Inform Source Files (*.inf;*.h)|*.inf;*.h|All Files (*.*)|*.*"),
    wxFD_SAVE,wxDefaultPosition,wxDefaultSize,_T("filedlg"));
    wxString path,name;
    if (fd->ShowModal() == wxID_OK ){
        path = fd->GetPath();
        name = fd->GetFilename();        
        if (wxFile::Exists(path)){
            wxMessageBox (ERRORS_FILEEXISTS, ERRORS_FILEACCESSERROR,  wxOK | wxICON_ERROR);
            return;
        }
        wxFile file;        
        file.Create(path, false, wxS_DEFAULT);
        Edit* stc = new Edit( auinotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER, path, this);
        stc->LoadFile (path);

        setNewStc(stc);

        OnClear();

        wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16));
        auinotebook->AddPage(stc  , name, true , page_bmp);
        //auinotebook->SetSelection(auinotebook->GetSelection()+1);
        OnUpdateTree();
    }
}

// MENU - SEARCH
void MyFrame::Search(wxCommandEvent &event){  //PL
    int position, len, newpos;
    wxString selected;
    // If there's a selection WIDE will search the selected String
    if (auinotebook->GetPageCount()==0) return;
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    if (e) {
        selected = e->GetSelectedText();
        position = e->GetSelectionEnd(); 
        len = e->GetLength();
        // Else, open a dialog
        if (selected==_T("")) {
            wxTextEntryDialog dialog(this,
                                     MESSAGES_SEARCH_ENTERSTRING,
                                     MESSAGES_SEARCH_SEARCHSTRING,
                                     lastSearch,
                                     wxOK | wxCANCEL);
        
            if (dialog.ShowModal() == wxID_OK) {
                selected = dialog.GetValue();
                position = e->GetCurrentPos();
            }
        }
        // Do the search
        if (selected!=_T("")) {
            lastSearch = selected;
            newpos = e->FindText(position,len,selected,0);
            // If not found, search from beginning 
            if (newpos < 0) newpos = e->FindText(0,position,selected,0);
            if (newpos >= 0) {
                int linea = e->LineFromPosition(newpos);
                e->GotoLine(linea);
                e->SetSelection(newpos,newpos+selected.Length());
            }
        }                
    }
}

void MyFrame::FindReplace(wxCommandEvent &event){   
    wxMessageBox (ERRORS_NOTIMPLEMENTED, MESSAGES_MESSAGE,  wxOK | wxICON_INFORMATION);           
}

void MyFrame::SearchBack(wxCommandEvent &event){   //PL
    int position, len, newpos;
    wxString selected;
    // If there's a selection WIDE will search the selected String
    if (auinotebook->GetPageCount()==0) return;
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    if (e) {
        selected = e->GetSelectedText();
        position = e->GetSelectionStart();
        len = e->GetLength();
        // Else, open a dialog
        if (selected==_T("")) {
            wxTextEntryDialog dialog(this,
                                     MESSAGES_SEARCH_ENTERSTRING,
                                     MESSAGES_SEARCH_SEARCHSTRING,
                                     lastSearch,
                                     wxOK | wxCANCEL);

            if (dialog.ShowModal() == wxID_OK) {
                selected = dialog.GetValue();
                position = e->GetCurrentPos();
            }
        }
        // Do the search
        if (selected!=_T("")) {
            lastSearch = selected;
            newpos = e->FindText(position-1,0,selected,0);
            // If not found, search from end
            if (newpos < 0) newpos = e->FindText(len,position,selected,0);
            if (newpos >= 0) {
                int linea = e->LineFromPosition(newpos);
                e->GotoLine(linea);
                e->SetSelection(newpos,newpos+selected.Length());
            }
        }
    }
}

void MyFrame::SearchObject(bool globalflag){  //PL
    int position, len, newpos;
    wxString selected;
    // If there's a selection WIDE will search the selected String
    if (auinotebook->GetPageCount()==0) return;
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    if (e) {
        selected = e->GetSelectedText();
        position = e->GetSelectionEnd();
        len = e->GetLength();
        // Else, open a dialog
        if (selected==_T("")) {
            wxString caption = MESSAGES_SEARCHOBJECT;
            if (globalflag) caption = MESSAGES_SEARCHOBJECTFILES;
            wxTextEntryDialog dialog(this,
                                     MESSAGES_SEARCHOBJECT_ENTEROBJECT,
                                     caption,
                                     lastObSearch,
                                     wxOK | wxCANCEL);

            if (dialog.ShowModal() == wxID_OK) {
                selected = dialog.GetValue();
                position = e->GetCurrentPos();
            }
        }
        // Do the search
        if (selected!=_T("")) {
            lastObSearch = selected;
            int pannelli = auinotebook->GetPageCount();
            int current = auinotebook->GetSelection();
            int searched = current;
            size_t k = 0;
            Edit* e;
            for (;;) {
                //Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
                e = (Edit*) auinotebook->GetPage(searched);
                wxString text = e->GetText();           
                if (showObjects) k = SearchRegularExpression(text, _T("\n+[ \t\f\v]*Object[ \t\n\r\f\v]+(->[ \t\n\r\f\v]+)*") + selected + _T("[ \t\n\r\f\v]"));
                if (showProject) for (size_t i = 0; (k==0) && (i<projclasses.GetCount()); i++) {
                    wxString regexp = _T("\n+[ \t\f\v]*")+projclasses[i]+_T("[ \t\n\r\f\v]+(->[ \t\n\r\f\v]+)*") + selected + _T("[ \t\n\r\f\v]");
                    //wxString regexp = "\n+[ \t\f\v]*NPC[ \t\n\r\f\v]+(->[ \t\n\r\f\v]+)*" + selected + "[ \t\n\r\f\v]";                
                    k = SearchRegularExpression(text, regexp);
                }
                if (showGlobals && (k==0)) k = SearchRegularExpression(text, _T("\n+[ \t\f\v]*Global[ \t\n\r\f\v]+") + selected + _T("[= \t\n\r\f\v]"));
                if (showConstants && (k==0)) k = SearchRegularExpression(text, _T("\n+[ \t\f\v]*Constant[ \t\n\r\f\v]+") + selected + _T("[ \t\n\r\f\v]"));
                if (showFunctions && (k==0)) k = SearchRegularExpression(text, _T("[\n\r][ \t\n\r\f\v]*\\[[ \t\n\r\f\v]*") + selected + _T(".*;"));
                if (showClasses && (k==0)) k = SearchRegularExpression(text, _T("\n+[ \t\f\v]*Class[ \t\n\r\f\v]+") + selected + _T("[ \t\n\r\f\v]"));
                if (showIncludes && (k==0)) k = SearchRegularExpression(text, _T("\n+[ \t\f\v]*Include[ \t\f\v]+\"") + selected + _T("(\x2eh)?\""));
                if (showVerbs && (k==0)) k = SearchRegularExpression(text, _T("\n+[ \t\f\v]*Verb[ \t\f\v]+\'") + selected +_T("\'"));                
                if (k) break;
                if (!globalflag) break;
                searched++; if (searched>=pannelli) searched=0;
                if (searched == current) break;
            }
            auinotebook->SetSelection(current);

            //lastSearch = selected;
            if (k) {
                if (searched != current) auinotebook->SetSelection(searched);
                e = (Edit*) auinotebook->GetPage(searched);
                position = k;
                len = e->GetLength();
                newpos = e->FindText(position,len,selected,0);
                int linea = e->LineFromPosition(newpos);
                e->GotoLine(linea);
                e->SetSelection(newpos,newpos+selected.Length());
            }
        }
    }
}

size_t MyFrame::SearchRegularExpression(wxString text, wxString pattern_global){
    wxRegEx re;
    if (!re.Compile(pattern_global, wxRE_ICASE|wxRE_NEWLINE) ){
        OnOutput(ERRORS_RE);
        return 0;
    }
    if ( re.Matches(text) ){
        size_t start, len;
        if (re.GetMatch(&start, &len, 0)){
          if (text[start-1]=='\"') {start--; len++; }
          return start;
        }
    }
    return 0;
}

void MyFrame::NextMarker(wxCommandEvent &event){
    if (auinotebook->GetPageCount()==0) return;
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    if (e) {
        int line = e->MarkerNext(e->GetCurrentLine()+1,0x01000000);     
        //wxMessageBox (wxString::Format("Sono qui linea %d",line), _("File access Error"),  wxOK | wxICON_ERROR);           
        if (line!=-1){
            // GotoLine
            e->GotoLine(line);
        }
        else{
            // Torno alla prima riga
            line = e->MarkerNext(1,0x01000000);     
            if (line!=-1){
                e->GotoLine(line);                
            }
            else{
                wxMessageBox (ERRORS_NOMARKERS, MESSAGES_WARNING,  wxOK | wxICON_INFORMATION);           
            }
        }
    } 
}

void MyFrame::GotoLine(wxCommandEvent &event){
    if (auinotebook->GetPageCount()==0) return;
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    if (e) {
        // Richiedo il numero di riga
        long lineNumber = wxGetNumberFromUser( _T(""),MESSAGES_LINENUMBER, MESSAGES_GOTOLINENUMBER,
                             1, 1, 999999999, this );
        if (lineNumber !=-1){
            e->GotoLine(lineNumber-1);
        }
    }
}

void MyFrame::ToggleMarker(wxCommandEvent &event){
    if (auinotebook->GetPageCount()==0) return;
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    if (e) {
        //e->MarkerDefine(24, wxSTC_MARK_BOXPLUS,wxNullColour,wxColour(_T("LIGHT GREY")));        
        //wxMessageBox (_("Sono qui"), _("File access Error"),  wxOK | wxICON_ERROR);        
        if (e->MarkerGet(e->GetCurrentLine())==0){
            // Add a new marker
            e->MarkerAdd(e->GetCurrentLine(), 24);
        }
        else{
            // Remove the current marker
            e->MarkerDelete(e->GetCurrentLine(), 24);
        }
    }
}

void MyFrame::ResetMarkers(wxCommandEvent &event){
    if (auinotebook->GetPageCount()==0) return;
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    if (e) {
        e->MarkerDeleteAll(-1);
    }
}


void MyFrame::SaveConfiguration() {
     pConfig->Write(_T("USEOTHERKEYWORDS"), useOtherkeywords);
     pConfig->Write(_T("USEDIRECTIVES"),    useDirectives);
     pConfig->Write(_T("USESTATEMENTS"),    useStatements);
     pConfig->Write(_T("USEINFORMATE"),     useInformate);
     pConfig->Write(_T("USECUSTOM1"),       useCustom1);
     pConfig->Write(_T("USECUSTOM2"),       useCustom2);
     pConfig->Write(_T("USECUSTOM3"),       useCustom3);

     pConfig->Write(_T("OBJECT_TREE_EXPAND_ALL_NODES"), expandAllNodes);
     pConfig->Write(_T("OBJECT_TREE_SHOW_OBJECTS"), showObjects);
     pConfig->Write(_T("OBJECT_TREE_SHOW_PROJECT"), showProject);
     pConfig->Write(_T("OBJECT_TREE_SHOW_GLOBALS"), showGlobals);
     pConfig->Write(_T("OBJECT_TREE_SHOW_CONSTANTS"), showConstants);
     pConfig->Write(_T("OBJECT_TREE_SHOW_FUNCTIONS"), showFunctions);
     pConfig->Write(_T("OBJECT_TREE_SHOW_CLASSES"), showClasses);
     pConfig->Write(_T("OBJECT_TREE_SHOW_INCLUDES"), showIncludes);
     pConfig->Write(_T("OBJECT_TREE_SHOW_VERBS"), showVerbs);

     pConfig->Write(_T("SHOWLINENUMBERS"), showLineNumber);
     pConfig->Write(_T("WRAPMODE"), wrapMode);
     pConfig->Write(_T("AUTOCOMPLETION"), autoCompleteSwitch);
     pConfig->Write(_T("REPLACEHOTKEYS"), hotkeysSwitch);
     
     pConfig->Write(_T("AUTOCOMPLETE_NUMBER"), autoCompleteNumber);
     pConfig->Write(_T("TABSIZE"), tabSize);
     
     // Window location
     int x,y,width,height;
     this->GetPosition(&x,&y);
     this->GetSize(&width,&height);
     pConfig->Write(_T("WINDOW_WIDTH"), width);     
     pConfig->Write(_T("WINDOW_HEIGHT"), height);     
     pConfig->Write(_T("WINDOW_LOCATION_X"), x);          
     pConfig->Write(_T("WINDOW_LOCATION_Y"), y);          
}



void MyFrame::LoadConfiguration() {
    
     // syntax stuff
     useOtherkeywords = pConfig->Read(_T("USEOTHERKEYWORDS"), 1l) != 0;
     useDirectives = pConfig->Read(_T("USEDIRECTIVES"), 1l) != 0;
     useStatements = pConfig->Read(_T("USESTATEMENTS"), 1l) != 0;
     useInformate = pConfig->Read(_T("USEINFORMATE"), 1l) != 0;
     useCustom1 = pConfig->Read(_T("USECUSTOM1"), 1l) != 0;
     useCustom2 = pConfig->Read(_T("USECUSTOM2"), 1l) != 0;
     useCustom3 = pConfig->Read(_T("USECUSTOM3"), 1l) != 0;

     expandAllNodes = pConfig->Read(_T("OBJECT_TREE_EXPAND_ALL_NODES"), 1l) != 0;
     showObjects    = pConfig->Read(_T("OBJECT_TREE_SHOW_OBJECTS"), 1l) != 0;
     showProject    = pConfig->Read(_T("OBJECT_TREE_SHOW_PROJECT"), 1l) != 0;
     showGlobals    = pConfig->Read(_T("OBJECT_TREE_SHOW_GLOBALS"), 1l) != 0;
     showConstants  = pConfig->Read(_T("OBJECT_TREE_SHOW_CONSTANTS"), 1l) != 0;
     showFunctions  = pConfig->Read(_T("OBJECT_TREE_SHOW_FUNCTIONS"), 1l) != 0;
     showClasses    = pConfig->Read(_T("OBJECT_TREE_SHOW_CLASSES"), 1l) != 0;
     showIncludes   = pConfig->Read(_T("OBJECT_TREE_SHOW_INCLUDES"), 1l) != 0;
     showVerbs      = pConfig->Read(_T("OBJECT_TREE_SHOW_VERBS"), 1l) != 0;

     showLineNumber     = pConfig->Read(_T("SHOWLINENUMBERS"), 1l) != 0;
     wrapMode           = pConfig->Read(_T("WRAPMODE"), 1l) != 0; 
     autoCompleteSwitch = pConfig->Read(_T("AUTOCOMPLETION"), 1l) != 0;
     hotkeysSwitch      = pConfig->Read(_T("REPLACEHOTKEYS"), 1l) != 0;
     
     autoCompleteNumber = pConfig->Read(_T("AUTOCOMPLETE_NUMBER"), 1l);
     tabSize = pConfig->Read(_T("TABSIZE"), 1l);
     
     
     // Inform Stuff
     informCompiler = pConfig->Read(_T("INFORMCOMPILER"), _T(""));
#ifdef __WINDOWS__     
     libraryPath=_T("+include_path=\"")+
                pConfig->Read(_T("LIBRARYPATH1"), _T(""))+_T("\",\"")+
                pConfig->Read(_T("LIBRARYPATH2"), _T(""))+_T("\",\"")+
                pConfig->Read(_T("LIBRARYPATH3"), _T(""))+_T("\"");                
#else     
     libraryPath=_T("+include_path=")+
                pConfig->Read(_T("LIBRARYPATH1"), _T(""))+_T(",")+
                pConfig->Read(_T("LIBRARYPATH2"), _T(""))+_T(",")+
                pConfig->Read(_T("LIBRARYPATH3"), _T(""))+_T("");                
#endif     
    bres = pConfig->Read(_T("BRESPATH"), _T(""));                
    blc  = pConfig->Read(_T("BLCPATH"), _T(""));
    bext = pConfig->Read(_T("BLORBEXTENSION"), _T(""));
    
    // TRANSLATION MESSAGES AND MENUES
    language = pConfig->Read(_T("LANGUAGE"), _T(""));

    // MENU_FILE    
    MENU_FILE             = pConfig->Read(language+_T("_")+_T("MENU_FILE"),_T(""))             !=_T("")?pConfig->Read(language+_T("_")+_T("MENU_FILE"),_T("")):_T("File");
    MENU_FILE_QUIT        = pConfig->Read(language+_T("_")+_T("MENU_FILE_QUIT"),_T(""))        !=_T("")?pConfig->Read(language+_T("_")+_T("MENU_FILE_QUIT"),_T("")):_T("Quit");
    MENU_FILE_NEWFILE     = pConfig->Read(language+_T("_")+_T("MENU_FILE_NEWFILE"),_T(""))     !=_T("")?pConfig->Read(language+_T("_")+_T("MENU_FILE_NEWFILE"),_T("")):_T("New File");
    MENU_FILE_OPENFILE    = pConfig->Read(language+_T("_")+_T("MENU_FILE_OPENFILE"),_T(""))    !=_T("")?pConfig->Read(language+_T("_")+_T("MENU_FILE_OPENFILE"),_T("")):_T("Open File");
    MENU_FILE_SAVEFILE    = pConfig->Read(language+_T("_")+_T("MENU_FILE_SAVEFILE"),_T(""))    !=_T("")?pConfig->Read(language+_T("_")+_T("MENU_FILE_SAVEFILE"),_T("")):_T("Save File");
    MENU_FILE_SAVEALL     = pConfig->Read(language+_T("_")+_T("MENU_FILE_SAVEALL"),_T(""))     !=_T("")?pConfig->Read(language+_T("_")+_T("MENU_FILE_SAVEALL"),_T("")):_T("Save All");
    MENU_FILE_NEXTTAB     = pConfig->Read(language+_T("_")+_T("MENU_FILE_NEXTTAB"),_T(""))     !=_T("")?pConfig->Read(language+_T("_")+_T("MENU_FILE_NEXTTAB"),_T("")):_T("Next Tab");
    MENU_FILE_PREVIOUSTAB = pConfig->Read(language+_T("_")+_T("MENU_FILE_PREVIOUSTAB"),_T("")) !=_T("")?pConfig->Read(language+_T("_")+_T("MENU_FILE_PREVIOUSTAB"),_T("")):_T("Previous Tab");
    
    // MENU EDIT
    MENU_EDIT             = pConfig->Read(language+_T("_")+_T("MENU_EDIT"),_T(""))             !=_T("")?pConfig->Read(language+_T("_")+_T("MENU_EDIT"),_T("")):_T("Edit");
    MENU_EDIT_COPY        = pConfig->Read(language+_T("_")+_T("MENU_EDIT_COPY"),_T(""))        !=_T("")?pConfig->Read(language+_T("_")+_T("MENU_EDIT_COPY"),_T("")):_T("Copy");
    MENU_EDIT_CUT         = pConfig->Read(language+_T("_")+_T("MENU_EDIT_CUT"),_T(""))         !=_T("")?pConfig->Read(language+_T("_")+_T("MENU_EDIT_CUT"),_T("")):_T("Cut");
    MENU_EDIT_PASTE       = pConfig->Read(language+_T("_")+_T("MENU_EDIT_PASTE"),_T(""))       !=_T("")?pConfig->Read(language+_T("_")+_T("MENU_EDIT_PASTE"),_T("")):_T("Paste");
    MENU_EDIT_UNDO        = pConfig->Read(language+_T("_")+_T("MENU_EDIT_UNDO"),_T(""))        !=_T("")?pConfig->Read(language+_T("_")+_T("MENU_EDIT_UNDO"),_T("")):_T("Undo");
    MENU_EDIT_REDO        = pConfig->Read(language+_T("_")+_T("MENU_EDIT_REDO"),_T(""))        !=_T("")?pConfig->Read(language+_T("_")+_T("MENU_EDIT_REDO"),_T("")):_T("Redo");
    MENU_EDIT_INDENT      = pConfig->Read(language+_T("_")+_T("MENU_EDIT_INDENT"),_T(""))      !=_T("")?pConfig->Read(language+_T("_")+_T("MENU_EDIT_INDENT"),_T("")):_T("Indent");
    MENU_EDIT_UNINDENT    = pConfig->Read(language+_T("_")+_T("MENU_EDIT_UNINDENT"),_T(""))    !=_T("")?pConfig->Read(language+_T("_")+_T("MENU_EDIT_UNINDENT"),_T("")):_T("Unindent");
    MENU_EDIT_COMMENT     = pConfig->Read(language+_T("_")+_T("MENU_EDIT_COMMENT"),_T(""))     !=_T("")?pConfig->Read(language+_T("_")+_T("MENU_EDIT_COMMENT"),_T("")):_T("Comment");
    MENU_EDIT_UNCOMMENT   = pConfig->Read(language+_T("_")+_T("MENU_EDIT_UNCOMMENT"),_T(""))   !=_T("")?pConfig->Read(language+_T("_")+_T("MENU_EDIT_UNCOMMENT"),_T("")):_T("Uncomment");


    // SEARCH MENU
    MENU_SEARCH = pConfig->Read(language+_T("_")+_T("MENU_SEARCH"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_SEARCH"),_T("")):_T("Search");
    MENU_SEARCH_FIND = pConfig->Read(language+_T("_")+_T("MENU_SEARCH_FIND"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_SEARCH_FIND"),_T("")):_T("Find");
    MENU_SEARCH_FIND_BACKWARDS = pConfig->Read(language+_T("_")+_T("MENU_SEARCH_FIND_BACKWARDS"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_SEARCH_FIND_BACKWARDS"),_T("")):_T("Find Backwards");
    MENU_SEARCH_FIND_REPLACE = pConfig->Read(language+_T("_")+_T("MENU_SEARCH_FIND_REPLACE"),_T("")) !=_T("")?pConfig->Read(language+_T("_")+_T("MENU_SEARCH_FIND_REPLACE"),_T("")):_T("Find & Replace");
    MENU_SEARCH_FIND_OBJECT = pConfig->Read(language+_T("_")+_T("MENU_SEARCH_FIND_OBJECT"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_SEARCH_FIND_OBJECT"),_T("")):_T("Find Object");
    MENU_SEARCH_FIND_OBJECT_IN_FILE = pConfig->Read(language+_T("_")+_T("MENU_SEARCH_FIND_OBJECT_IN_FILE"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_SEARCH_FIND_OBJECT_IN_FILE"),_T("")):_T("Find Object in File");

    MENU_SEARCH_FIND_GOTO_LINE = pConfig->Read(language+_T("_")+_T("MENU_SEARCH_FIND_GOTO_LINE"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_SEARCH_FIND_GOTO_LINE"),_T("")):_T("Goto Line");
    MENU_SEARCH_FIND_NEXT_MARKER = pConfig->Read(language+_T("_")+_T("MENU_SEARCH_FIND_NEXT_MARKER"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_SEARCH_FIND_NEXT_MARKER"),_T("")):_T("Next Marker");
    MENU_SEARCH_FIND_TOGGLE_MARKER = pConfig->Read(language+_T("_")+_T("MENU_SEARCH_FIND_TOGGLE_MARKER"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_SEARCH_FIND_TOGGLE_MARKER"),_T("")):_T("Toggle Marker");
    MENU_SEARCH_FIND_RESET_MARKERS = pConfig->Read(language+_T("_")+_T("MENU_SEARCH_FIND_RESET_MARKERS"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_SEARCH_FIND_RESET_MARKERS"),_T("")):_T("Reset Markers");

    // PROJECT MENU
    MENU_PROJECT = pConfig->Read(language+_T("_")+_T("MENU_PROJECT"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_PROJECT"),_T("")):_T("Project");
    MENU_PROJECT_OPEN = pConfig->Read(language+_T("_")+_T("MENU_PROJECT_OPEN"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_PROJECT_OPEN"),_T("")):_T("Open Project"); 
    MENU_PROJECT_SAVE = pConfig->Read(language+_T("_")+_T("MENU_PROJECT_SAVE"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_PROJECT_SAVE"),_T("")):_T("Save Project"); 
    MENU_PROJECT_CLOSE = pConfig->Read(language+_T("_")+_T("MENU_PROJECT_CLOSE"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_PROJECT_CLOSE"),_T("")):_T("Close Project");

    
    // LANGUAGE MENU
    MENU_LANGUAGE_ENGLISH = _T("English");
    MENU_LANGUAGE_ITALIAN = _T("Italian");
    MENU_LANGUAGE_SPANISH = _T("Spanish");
    MENU_LANGUAGE_GERMAN = _T("German");
    MENU_LANGUAGE_FRENCH = _T("French");
    
    
    // ZCODE MENU
    MENU_ZCODE = pConfig->Read(language+_T("_")+_T("MENU_ZCODE"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_ZCODE"),_T("")):_T("Zcode");               
    MENU_ZCODE_COMPILEZCODE = pConfig->Read(language+_T("_")+_T("MENU_ZCODE_COMPILEZCODE"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_ZCODE_COMPILEZCODE"),_T("")):_T("Compile Zcode");  
    MENU_ZCODE_RUNZCODE = pConfig->Read(language+_T("_")+_T("MENU_ZCODE_RUNZCODE"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_ZCODE_RUNZCODE"),_T("")):_T("Run Zcode");      
    MENU_ZCODE_ZCODE5 = pConfig->Read(language+_T("_")+_T("MENU_ZCODE_ZCODE5"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_ZCODE_ZCODE5"),_T("")):_T("ZCode version 5");        
    MENU_ZCODE_ZCODE8 = pConfig->Read(language+_T("_")+_T("MENU_ZCODE_ZCODE8"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_ZCODE_ZCODE8"),_T("")):_T("ZCode version 8");        
    MENU_ZCODE_COMPILEZCODE_BLORB = pConfig->Read(language+_T("_")+_T("MENU_ZCODE_COMPILEZCODE_BLORB"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_ZCODE_COMPILEZCODE_BLORB"),_T("")):_T("Create Zcode blorb");
    MENU_ZCODE_RUNZCODE_BLORB = pConfig->Read(language+_T("_")+_T("MENU_ZCODE_RUNZCODE_BLORB"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_ZCODE_RUNZCODE_BLORB"),_T("")):_T("Run Zcode blorb");

    // GLULX MENU
    MENU_GLULX = pConfig->Read(language+_T("_")+_T("MENU_GLULX"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_GLULX"),_T("")):_T("Glulx");
    MENU_GLULX_COMPILEULX = pConfig->Read(language+_T("_")+_T("MENU_GLULX_COMPILEULX"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_GLULX_COMPILEULX"),_T("")):_T("Compile Ulx");
    MENU_GLULX_RUNULX = pConfig->Read(language+_T("_")+_T("MENU_GLULX_RUNULX"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_GLULX_RUNULX"),_T("")):_T("Run Ulx");
    MENU_GLULX_CREATERESOURCES = pConfig->Read(language+_T("_")+_T("MENU_GLULX_CREATERESOURCES"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_GLULX_CREATERESOURCES"),_T("")):_T("Create Resources");
    MENU_GLULX_CREATEBLORBFILE = pConfig->Read(language+_T("_")+_T("MENU_GLULX_CREATEBLORBFILE"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_GLULX_CREATEBLORBFILE"),_T("")):_T("Create Blorb File");
    MENU_GLULX_BUILDALLBLORBFILE = pConfig->Read(language+_T("_")+_T("MENU_GLULX_BUILDALLBLORBFILE"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_GLULX_BUILDALLBLORBFILE"),_T("")):_T("Build all Blorb file");
    MENU_GLULX_RUNBLORB = pConfig->Read(language+_T("_")+_T("MENU_GLULX_RUNBLORB"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_GLULX_RUNBLORB"),_T("")):_T("Run Blorb");
                 
    // Object Tree Menu
    MENU_OBJECTTREE = pConfig->Read(language+_T("_")+_T("MENU_OBJECTTREE"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_OBJECTTREE"),_T("")):_T("Object Tree");
    MENU_OBJECTTREE_REFRESHTREE = pConfig->Read(language+_T("_")+_T("MENU_OBJECTTREE_REFRESHTREE"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_OBJECTTREE_REFRESHTREE"),_T("")):_T("Refresh tree");
    MENU_OBJECTTREE_EXPAND_NODES = pConfig->Read(language+_T("_")+_T("MENU_OBJECTTREE_EXPAND_NODES"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_OBJECTTREE_EXPAND_NODES"),_T("")):_T("Expand always all nodes");
    MENU_OBJECTTREE_SHOW_OBJECTS = pConfig->Read(language+_T("_")+_T("MENU_OBJECTTREE_SHOW_OBJECTS"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_OBJECTTREE_SHOW_OBJECTS"),_T("")):_T("Show Objects");
    MENU_OBJECTTREE_SHOW_PROJECT_DEFS = pConfig->Read(language+_T("_")+_T("MENU_OBJECTTREE_SHOW_PROJECT_DEFS"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_OBJECTTREE_SHOW_PROJECT_DEFS"),_T("")):_T("Show Project Defs");        
    MENU_OBJECTTREE_SHOW_GLOBALS = pConfig->Read(language+_T("_")+_T("MENU_OBJECTTREE_SHOW_GLOBALS"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_OBJECTTREE_SHOW_GLOBALS"),_T("")):_T("Show Globals");
    MENU_OBJECTTREE_SHOW_CONSTANTS = pConfig->Read(language+_T("_")+_T("MENU_OBJECTTREE_SHOW_CONSTANTS"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_OBJECTTREE_SHOW_CONSTANTS"),_T("")):_T("Show Constants");
    MENU_OBJECTTREE_SHOW_FUNCTIONS = pConfig->Read(language+_T("_")+_T("MENU_OBJECTTREE_SHOW_FUNCTIONS"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_OBJECTTREE_SHOW_FUNCTIONS"),_T("")):_T("Show Functions");
    MENU_OBJECTTREE_SHOW_CLASSES = pConfig->Read(language+_T("_")+_T("MENU_OBJECTTREE_SHOW_CLASSES"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_OBJECTTREE_SHOW_CLASSES"),_T("")):_T("Show Classes");             
    MENU_OBJECTTREE_SHOW_INCLUDES = pConfig->Read(language+_T("_")+_T("MENU_OBJECTTREE_SHOW_INCLUDES"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_OBJECTTREE_SHOW_INCLUDES"),_T("")):_T("Show Includes");
    MENU_OBJECTTREE_SHOW_VERBS = pConfig->Read(language+_T("_")+_T("MENU_OBJECTTREE_SHOW_VERBS"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_OBJECTTREE_SHOW_VERBS"),_T("")):_T("Show Verbs");

    // Options Menu                      
    MENU_OPTIONS = pConfig->Read(language+_T("_")+_T("MENU_OPTIONS"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_OPTIONS"),_T("")):_T("Options");
    MENU_OPTIONS_SHOWLINENUMBER = pConfig->Read(language+_T("_")+_T("MENU_OPTIONS_SHOWLINENUMBER"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_OPTIONS_SHOWLINENUMBER"),_T("")):_T("Show line numbers");
    MENU_OPTIONS_WRAPMODE = pConfig->Read(language+_T("_")+_T("MENU_OPTIONS_WRAPMODE"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_OPTIONS_WRAPMODE"),_T("")):_T("Wrap Mode");
    MENU_OPTIONS_AUTOCOMPLETE = pConfig->Read(language+_T("_")+_T("MENU_OPTIONS_AUTOCOMPLETE"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_OPTIONS_AUTOCOMPLETE"),_T("")):_T("Autocomplete");
    MENU_OPTIONS_HOTKEYSREPLACING = pConfig->Read(language+_T("_")+_T("MENU_OPTIONS_HOTKEYSREPLACING"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_OPTIONS_HOTKEYSREPLACING"),_T("")):_T("Hotkeys replacing");
        
    // Documents Menu
    MENU_DOCUMENTS = pConfig->Read(language+_T("_")+_T("MENU_DOCUMENTS"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_DOCUMENTS"),_T("")):_T("Documents");

    // Help Menu    
    MENU_HELP = pConfig->Read(language+_T("_")+_T("MENU_HELP"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_HELP"),_T("")):_T("Help");    
    MENU_HELP_LICENSE = pConfig->Read(language+_T("_")+_T("MENU_HELP_LICENSE"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_HELP_LICENSE"),_T("")):_T("License");
    MENU_HELP_ABOUT = pConfig->Read(language+_T("_")+_T("MENU_HELP_ABOUT"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MENU_HELP_ABOUT"),_T("")):_T("About Wide");
                   
    // GENERIC MESSAGES AND ERRORS
    MESSAGES_READY = pConfig->Read(language+_T("_")+_T("MESSAGES_READY"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_READY"),_T("")):_T("Ready");    
    MESSAGES_OUTPUTWINDOW = pConfig->Read(language+_T("_")+_T("MESSAGES_OUTPUTWINDOW"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_OUTPUTWINDOW"),_T("")):_T("Output Window");    
    MESSAGES_SEARCH_ENTERSTRING = pConfig->Read(language+_T("_")+_T("MESSAGES_SEARCH_ENTERSTRING"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_SEARCH_ENTERSTRING"),_T("")):_T("Enter a string to find:");    
    MESSAGES_SEARCH_SEARCHSTRING = pConfig->Read(language+_T("_")+_T("MESSAGES_SEARCH_SEARCHSTRING"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_SEARCH_SEARCHSTRING"),_T("")):_T("Search String");    
    MESSAGES_MESSAGE = pConfig->Read(language+_T("_")+_T("MESSAGES_MESSAGE"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_MESSAGE"),_T("")):_T("Message");    
    MESSAGES_SEARCHOBJECT = pConfig->Read(language+_T("_")+_T("MESSAGES_SEARCHOBJECT"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_SEARCHOBJECT"),_T("")):_T("Search Object");    
    MESSAGES_SEARCHOBJECTFILES = pConfig->Read(language+_T("_")+_T("MESSAGES_SEARCHOBJECTFILES"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_SEARCHOBJECTFILES"),_T("")):_T("Search Object in files");    
    MESSAGES_SEARCHOBJECT_ENTEROBJECT= pConfig->Read(language+_T("_")+_T("MESSAGES_SEARCHOBJECT_ENTEROBJECT"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_SEARCHOBJECT_ENTEROBJECT"),_T("")):_T("Enter an object to find:");    
    MESSAGES_LINENUMBER = pConfig->Read(language+_T("_")+_T("MESSAGES_LINENUMBER"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_LINENUMBER"),_T("")):_T("Line number:");    
    MESSAGES_GOTOLINENUMBER = pConfig->Read(language+_T("_")+_T("MESSAGES_GOTOLINENUMBER"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_GOTOLINENUMBER"),_T("")):_T("Goto Line number");    
    MESSAGES_FILESAVED = pConfig->Read(language+_T("_")+_T("MESSAGES_FILESAVED"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_FILESAVED"),_T("")):_T("File saved");    
    MESSAGES_TEXTNOTSAVED = pConfig->Read(language+_T("_")+_T("MESSAGES_TEXTNOTSAVED"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_TEXTNOTSAVED"),_T("")):_T("Text is not saved, save before closing?");    
    MESSAGES_FILEOPENED = pConfig->Read(language+_T("_")+_T("MESSAGES_FILEOPENED"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_FILEOPENED"),_T("")):_T("File is already opened");    
    MESSAGES_WARNING = pConfig->Read(language+_T("_")+_T("MESSAGES_WARNING"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_WARNING"),_T("")):_T("Warning");    
    MESSAGES_REMOVEMAINFILE = pConfig->Read(language+_T("_")+_T("MESSAGES_REMOVEMAINFILE"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_REMOVEMAINFILE"),_T("")):_T("Remove Main File");    
    MESSAGES_MAINFILEREMOVED = pConfig->Read(language+_T("_")+_T("MESSAGES_MAINFILEREMOVED"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_MAINFILEREMOVED"),_T("")):_T("Main file, classes and keywords removed");    
    MESSAGES_NOPROJECTOPENED = pConfig->Read(language+_T("_")+_T("MESSAGES_NOPROJECTOPENED"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_NOPROJECTOPENED"),_T("")):_T("No Project opened");    
    MESSAGES_COMPILEULX = pConfig->Read(language+_T("_")+_T("MESSAGES_COMPILEULX"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_COMPILEULX"),_T("")):_T("Compiling in ULX format...");    
    MESSAGES_COMPILEZCODE = pConfig->Read(language+_T("_")+_T("MESSAGES_COMPILEZCODE"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_COMPILEZCODE"),_T("")):_T("Compiling Z-Code...");    
    MESSAGES_CREATINGBLORB = pConfig->Read(language+_T("_")+_T("MESSAGES_CREATINGBLORB"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_CREATINGBLORB"),_T("")):_T("Creating blorb file...");    
    MESSAGES_CREATINGRESOURCEZBLORB = pConfig->Read(language+_T("_")+_T("MESSAGES_CREATINGRESOURCEZBLORB"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_CREATINGRESOURCEZBLORB"),_T("")):_T("Creating Resources for z-blorb file...");    
    MESSAGES_CREATINGZBLORB = pConfig->Read(language+_T("_")+_T("MESSAGES_CREATINGZBLORB"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_CREATINGZBLORB"),_T("")):_T("Creating z-blorb file...");    
    MESSAGES_CREATINGRESOURCEBLORB  = pConfig->Read(language+_T("_")+_T("MESSAGES_CREATINGRESOURCEBLORB"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_CREATINGRESOURCEBLORB"),_T("")):_T("Creating Resources for blorb file...");    
    MESSAGES_RUNZBLORB = pConfig->Read(language+_T("_")+_T("MESSAGES_RUNZBLORB"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_RUNZBLORB"),_T("")):_T("Running z-blorb...");    
    MESSAGES_RUNZCODE = pConfig->Read(language+_T("_")+_T("MESSAGES_RUNZCODE"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_RUNZCODE"),_T("")):_T("Running zcode...");    
    MESSAGES_RUNBLORB = pConfig->Read(language+_T("_")+_T("MESSAGES_RUNBLORB"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_RUNBLORB"),_T("")):_T("Running blorb...");    
    MESSAGES_RUNGLULX = pConfig->Read(language+_T("_")+_T("MESSAGES_RUNGLULX"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_RUNGLULX"),_T("")):_T("Running glulx...");    
    MESSAGES_SAVINGALL = pConfig->Read(language+_T("_")+_T("MESSAGES_SAVINGALL"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_SAVINGALL"),_T("")):_T("Saving all...");    
    MESSAGES_USINGMAINFILE = pConfig->Read(language+_T("_")+_T("MESSAGES_USINGMAINFILE"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("MESSAGES_USINGMAINFILE"),_T("")):_T("Using MainFile:");    
    ERRORS_ERROR = pConfig->Read(language+_T("_")+_T("ERRORS_ERROR"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("ERRORS_ERROR"),_T("")):_T("Error");    
    ERRORS_RE = pConfig->Read(language+_T("_")+_T("ERRORS_RE"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("ERRORS_RE"),_T("")):_T("Errors in Regular Expression");    
    ERRORS_FILEEXISTS = pConfig->Read(language+_T("_")+_T("ERRORS_FILEEXISTS"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("ERRORS_FILEEXISTS"),_T("")):_T("The file already exists");    
    ERRORS_FILEACCESSERROR = pConfig->Read(language+_T("_")+_T("ERRORS_FILEACCESSERROR"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("ERRORS_FILEACCESSERROR"),_T("")):_T("File access Error");    
    ERRORS_NOTIMPLEMENTED = pConfig->Read(language+_T("_")+_T("ERRORS_NOTIMPLEMENTED"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("ERRORS_NOTIMPLEMENTED"),_T("")):_T("Not yet implemented");    
    ERRORS_NOMARKERS = pConfig->Read(language+_T("_")+_T("ERRORS_NOMARKERS"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("ERRORS_NOMARKERS"),_T("")):_T("No Markers found");    
    ERRORS_SAVEERROR = pConfig->Read(language+_T("_")+_T("ERRORS_SAVEERROR"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("ERRORS_SAVEERROR"),_T("")):_T("Text could not be saved!");    
    ERRORS_CLOSEABORT = pConfig->Read(language+_T("_")+_T("ERRORS_CLOSEABORT"),_T(""))!=_T("")?pConfig->Read(language+_T("_")+_T("ERRORS_CLOSEABORT"),_T("")):_T("Close abort");    
}   
    
    
// Eventi sul singolo EDIT
void MyFrame::OnSingleEdit (wxCommandEvent &evt) {
    // Recupero l'edit attuale e processo l'evento
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    if (e) {
        e->ProcessEvent (evt);
    }        
}


// Save file
void MyFrame::OnSaveFile (wxCommandEvent &evt) {
    if (auinotebook->GetPageCount()==0){
        return;
    }
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());    
    if (!e->Modified()) return;
    /*PL    wxMessageBox (_("There is nothing to save!"), _("Save file"),
                      wxOK | wxICON_EXCLAMATION);
        return;
    } */
    wxString nomeprima = e->GetFilename();
    e->SaveFile();
    
    // Aggiorno il nome del tab (in caso di rinominazione del file
    wxString nome = e->GetFilename();
//    wxString sep = "/";
    if (nome!=nomeprima){
        nome = nome.Mid(nome.Find('\\',true)+1,nome.Len());
        auinotebook->SetPageText(auinotebook->GetSelection(), nome);
    }
    
    // Aggiorno la status bar
    GetStatusBar()->SetStatusText(MESSAGES_FILESAVED);
    
    // Aggiorno anche l'albero degli oggetti
    OnUpdateTree();
}

void MyFrame::OnLanguage(wxCommandEvent &event) {
    int id = event.GetId();
    switch (id)
    {
        case myID_ENGLISH: 
            pConfig->Write(_T("LANGUAGE"), _T("ENGLISH"));          
            break;
        case myID_ITALIAN: 
            pConfig->Write(_T("LANGUAGE"), _T("ITALIAN"));          
            break;
        case myID_SPANISH: 
            pConfig->Write(_T("LANGUAGE"), _T("SPANISH"));          
            break;
        case myID_GERMAN: 
            pConfig->Write(_T("LANGUAGE"), _T("GERMAN"));          
            break;
        case myID_FRENCH: 
            pConfig->Write(_T("LANGUAGE"), _T("FRENCH"));          
            break;
    }
    wxMessageBox (_("Restart WIDE to apply"), _("Restart"),  wxOK | wxICON_EXCLAMATION);         
}

void MyFrame::OnSaveAll (wxCommandEvent &evt) {     //PL
    int pannelli = auinotebook->GetPageCount();
    for (int i=0; i<=pannelli-1 ;i++) { 
        Edit* e = (Edit*) auinotebook->GetPage(i);
        wxString titolo = auinotebook->GetPageText(i);
        if (e) {
            if (e->Modified()) e->SaveFile();
            if (e->Modified()) {
                wxMessageBox (ERRORS_SAVEERROR, ERRORS_CLOSEABORT,
                 wxOK | wxICON_EXCLAMATION);
                continue;
            }
        }
    }
    // Aggiorno la status bar
    GetStatusBar()->SetStatusText(MESSAGES_FILESAVED);

    // Aggiorno anche l'albero degli oggetti
    OnUpdateTree();
}


void MyFrame::OnNextPage(wxCommandEvent &evt) {
    int pannelli = auinotebook->GetPageCount();
    int current = auinotebook->GetSelection();
    current++; if (current>=pannelli) current=0;
    auinotebook->SetSelection(current);
}


void MyFrame::OnPreviousPage(wxCommandEvent &evt) {
    int pannelli = auinotebook->GetPageCount();
    int current = auinotebook->GetSelection();
    current--; if (current<0) current=pannelli-1;
    auinotebook->SetSelection(current);
}


// Impostazioni Object Tree
void MyFrame::OnRefreshTree(wxCommandEvent &event){
    OnUpdateTree();     
}

// Impostazioni Object Tree
void MyFrame::RefreshTree(){
    OnUpdateTree();     
}

void MyFrame::OnSyntax(wxCommandEvent &event){
    int id = event.GetId();
    switch (id)
    {
        case ID_UseDirectives:    useDirectives=event.IsChecked();     break;
        case ID_UseStatements:    useStatements=event.IsChecked();     break;
        case ID_UseOtherkeywords: useOtherkeywords=event.IsChecked();  break;        
        case ID_UseInformate:     useInformate=event.IsChecked();      break;        
        case ID_UseCustom1:       useCustom1=event.IsChecked();        break;        
        case ID_UseCustom2:       useCustom2=event.IsChecked();        break;                        
        case ID_UseCustom3:       useCustom3=event.IsChecked();        break;        
    }
}

void MyFrame::OnOptions(wxCommandEvent &event){
    int id = event.GetId();
//    wxMessageBox (_("Sono qui!"), _("Close abort"),  wxOK | wxICON_EXCLAMATION);
    switch (id)
    {
        case ID_ShowObject: showObjects=event.IsChecked(); OnUpdateTree(); break;
        case ID_ShowProject: showProject=event.IsChecked(); OnUpdateTree(); break;
        case ID_ShowGlobal: showGlobals=event.IsChecked(); OnUpdateTree(); break;        
        case ID_ShowConstant: showConstants=event.IsChecked(); OnUpdateTree(); break;        
        case ID_ShowFunction: showFunctions=event.IsChecked(); OnUpdateTree(); break;        
        case ID_ShowClass: showClasses=event.IsChecked(); OnUpdateTree(); break;                        
        case ID_ShowInclude: showIncludes=event.IsChecked(); OnUpdateTree(); break;        
        case ID_ShowVerb: showVerbs=event.IsChecked(); OnUpdateTree(); break;                 
        case ID_ExpandAllAlways: expandAllNodes=event.IsChecked(); OnUpdateTree(); break;                                

    }
}

// MENU DOC
void MyFrame::OnViewDoc(wxCommandEvent &event){
    int id = event.GetId();
//    wxMessageBox (_("Sono qui!"), _("Close abort"),  wxOK | wxICON_EXCLAMATION);
    wxString documento;
    wxString comando;
    switch (id)
    {
        case ID_Doc1: documento = pConfig->Read(_T("PDF1_PATH"), _T("")); break;
        case ID_Doc2: documento = pConfig->Read(_T("PDF2_PATH"), _T("")); break;
        case ID_Doc3: documento = pConfig->Read(_T("PDF3_PATH"), _T("")); break;
        case ID_Doc4: documento = pConfig->Read(_T("PDF4_PATH"), _T("")); break;
        case ID_Doc5: documento = pConfig->Read(_T("PDF5_PATH"), _T("")); break;
        case ID_Doc6: documento = pConfig->Read(_T("PDF6_PATH"), _T("")); break;
        case ID_Doc7: documento = pConfig->Read(_T("PDF7_PATH"), _T("")); break;
        case ID_Doc8: documento = pConfig->Read(_T("PDF8_PATH"), _T("")); break;
    }
    
    // Visualizzo il pdf o word   
    //wxString comando = pConfig->Read("PDFREADER", _T(""))+" \""+documento+"\"";
    wxString pdfreader = pConfig->Read(_T("PDFREADER"), _T(""));
    if (pdfreader == _T("")) {
        comando = _T("start \"\" \"") + documento + _T("\"");
        comando.Replace(_T("/"),_T("\\"),true);
        wxShell(comando);
    }
    else {
        comando = _T("\"") + pdfreader + _T("\" \"") + documento + _T("\"");
        wxExecute(comando);
    }
}


// MENU ZCODE  ******************************************************
// MENU - SEARCH
void MyFrame::OnZcodeVersion(wxCommandEvent &event){
    int id = event.GetId();
    switch (id)
    {
        case ID_ZcodeVersion5: 
            zcodeversion=_T(".z5"); 
            zcodeswitch=_T("-v5");
            break;
        case ID_ZcodeVersion8: 
            zcodeversion=_T(".z8"); 
            zcodeswitch=_T("-v8");            
            break;
    }
}
// Create Blb
void MyFrame::OnCreateBlb (wxCommandEvent &event) {
    if (auinotebook->GetPageCount()==0){ return; }
    if (event.GetId() != ID_MakeAllBlb) OnClear();
    OnOutput(MESSAGES_CREATINGBLORB+_T("\n"));
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    wxString blcFile, blbFile;
    if (mainFile==_T("")){
        blcFile = e->GetFilename();
        blbFile = e->GetFilename();
    }
    else{
        OnOutput(MESSAGES_USINGMAINFILE+_T(" ")+mainFile+_T("\n"));
        blcFile = mainFile;
        blbFile = mainFile;
    }
    
#ifdef __WINDOWS__     
    wxString wpath=wxPathOnly(blbFile);
    wxString mpath = wxGetCwd();
    wxSetWorkingDirectory(wpath);
#endif      
    


    blcFile.Replace(_T(".inf"), _T(".blc"), true);
    blbFile.Replace(_T(".inf"), _T(".")+bext, true);
    //wxString comando =  _T("\"")+blc +_T("\"")+_T(" ")+_T("\"")+blcFile+_T("\"")+_T(" ")+_T("\"")+blbFile+_T("\"");
    wxString comando =  blc +_T(" ")+blcFile+_T(" ")+blbFile;

    //wxString comando =  blc +" "+blcFile+" "+blbFile;    
    wxArrayString output;
    if ( wxExecute(comando, output) != 0 )
    {
        OnOutput(comando);
        OnOutput(_T("\n"));
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            OnOutput(output[n].c_str());
            OnOutput(_T("\n"));
        }
        OnOutput(ERRORS_ERROR+_T(".\n"));
        inform_error = true;
    }
    else
    {
        OnOutput(comando);
        OnOutput(_T("\n"));
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            OnOutput(output[n].c_str());
            OnOutput(_T("\n"));
        }
        OnOutput(_T("Ok.\n"));
        inform_error = false;
    }    
#ifdef __WINDOWS__     
    wxSetWorkingDirectory(mpath);    
#endif  
    
}

void MyFrame::OnCreateRes (wxCommandEvent &event) {
    if (auinotebook->GetPageCount()==0){ return; }
    if (event.GetId() != ID_MakeAllBlb) OnClear();
    OnOutput(MESSAGES_CREATINGRESOURCEBLORB+_T("\n"));
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    wxString bresFile;
    if (mainFile==_T("")){
        bresFile = e->GetFilename();
    }
    else{
        OnOutput(MESSAGES_USINGMAINFILE+_T(" ")+mainFile+_T("\n"));
        bresFile = mainFile;
    }
    bresFile.Replace(_T(".inf"), _T(""), true);
    wxString comando =  _T("\"")+bres +_T("\"")+_T(" ")+_T("\"")+bresFile+_T("\"");
    wxArrayString output;
    if ( wxExecute(comando, output) != 0 )
    {
        OnOutput(comando);
        OnOutput(_T("\n"));
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            OnOutput(output[n].c_str());
            OnOutput(_T("\n"));
        }

        OnOutput(ERRORS_ERROR+_T(".\n"));
        inform_error = true;
    }
    else
    {
        OnOutput(comando);
        OnOutput(_T("\n"));
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            OnOutput(output[n].c_str());
            OnOutput(_T("\n"));
        }
        OnOutput(_T("Ok.\n"));
        inform_error = false;
    }
}

// Make All Blb
void MyFrame::OnMakeAllBlb (wxCommandEvent &event) {
    if (auinotebook->GetPageCount()==0){ return; }
    OnClear();
    OnCreateRes(event); 
    if (inform_error) return;
    OnCompileUlx(event); 
    if (inform_error) return;
    OnCreateBlb(event);
}



// Run BLB
void MyFrame::OnRunBlb (wxCommandEvent &event) {
    if (auinotebook->GetPageCount()==0){ return; }
    OnClear();
    OnOutput(MESSAGES_RUNBLORB+_T("\n"));
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    
    // Running the MAIN FILE
    wxString nome;
    if (mainFile==_T("")){
        nome = e->GetFilename();
    }
    else{
        OnOutput(MESSAGES_USINGMAINFILE+_T(" ")+mainFile+_T("\n"));
        nome = mainFile;
    }
    nome.Replace(_T(".inf"), _T(".")+bext, true);
    wxString comando =  pConfig->Read(_T("GLULXINTERPRETER"), _T("")) +_T(" ")+_T("\"")+nome+_T("\"");
    OnOutput(comando+_T("\n"));
    wxArrayString output;
    wxExecute(comando);
}

// Run Ulx
// Should I Run the MAIN FILE
void MyFrame::OnRunUlx (wxCommandEvent &event) {
    if (auinotebook->GetPageCount()==0){ return; }
    OnClear();
    OnOutput(MESSAGES_RUNGLULX+_T("\n"));
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());

    // Running the MAIN FILE
    wxString nome;
    if (mainFile==_T("")){
        nome = e->GetFilename();
    }
    else{
        OnOutput(MESSAGES_USINGMAINFILE+_T(" ")+mainFile+_T("\n"));
        nome = mainFile;
    }        
    nome.Replace(_T(".inf"), _T(".ulx"), true);
    wxString comando =  pConfig->Read(_T("GLULXINTERPRETER"), _T("")) +_T(" ")+_T("\"")+nome+_T("\"");
    OnOutput(comando+_T("\n"));
    wxArrayString output;
    wxExecute(comando);
}

// Compile ULX file
void MyFrame::OnCompileUlx (wxCommandEvent &event) {
    // Se non ho finetre aperte esco
    //PL: L'ho fixata io, ma non mi ricordo come
    //    probabilmente copiando quella per z-code
    if (auinotebook->GetPageCount()==0){ return; }

    // Azzero la console
    if (event.GetId() != ID_MakeAllBlb) OnClear();
    
    // AS: pulisco la console
    OnClear();
    
    // PL: Salvo tutto
    OnOutput(MESSAGES_SAVINGALL+_T("\n"));
    //wxCommandEvent subev;
    OnSaveAll(event);
    
    OnOutput(MESSAGES_COMPILEULX+_T("\n"));
    wxArrayString output;

    // Recupero il nome del file da compilare
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    wxString nome,ulx;
    if (mainFile==_T("")){
        nome = e->GetFilename();
        ulx = e->GetFilename();
    }
    else{
        OnOutput(MESSAGES_USINGMAINFILE+_T(" ")+mainFile+_T("\n"));
        nome = mainFile;
        ulx = mainFile;
    }
    ulx.Replace(_T(".inf"), _T(".ulx"), true);
    wxString comando =  informCompiler +_T(" -G ")+libraryPath+_T(" \"")+nome+_T("\" ")+_T("\"")+ulx+_T("\"");

    //if ( wxExecute(_T("./inform.exe twocol.inf"), output) != 0 )
    if ( wxExecute(comando, output) != 0 )
    {
        OnOutput(comando);
        OnOutput(_T("\n"));
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            //wxPrintf(_T("\t%s\n"), output[n].c_str());
            OnOutput(output[n].c_str());
            OnOutput(_T("\n"));
        }

        OnOutput(ERRORS_ERROR+_T(".\n"));
        inform_error = true;

    }
    else
    {
        OnOutput(comando);
        OnOutput(_T("\n"));
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            //wxPrintf(_T("\t%s\n"), output[n].c_str());
            OnOutput(output[n].c_str());
            OnOutput(_T("\n"));
        }
        OnOutput(_T("Ok.\n"));
        inform_error = false;
    }
}

// Create ZBLB
void MyFrame::OnCreateZBlb (wxCommandEvent &event) {
    if (auinotebook->GetPageCount()==0){ return; }
    OnClear();
    OnOutput(MESSAGES_CREATINGRESOURCEZBLORB+_T("\n"));
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    wxString idFile;
    if (mainFile==_T("")){
        idFile = e->GetFilename();
    }
    else{
        OnOutput(MESSAGES_USINGMAINFILE+_T(" ")+mainFile+_T("\n"));
        idFile = mainFile;
    }
    idFile.Replace(_T(".inf"), _T(""), true);
    wxString comando =  _T("\"")+bres +_T("\" \"")+idFile+_T(".zres\" \"")+idFile+_T(".zblc\" 2del.bli");
    wxArrayString output;
    if ( wxExecute(comando, output) != 0 )
    {
        OnOutput(comando);
        OnOutput(_T("\n"));
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            OnOutput(output[n].c_str());
            OnOutput(_T("\n"));
        }

        OnOutput(ERRORS_ERROR+_T(".\n"));
        inform_error = true;
    }
    else
    {
        OnOutput(comando);
        OnOutput(_T("\n"));
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            OnOutput(output[n].c_str());
            OnOutput(_T("\n"));
        }
        OnOutput(_T("Ok.\n"));
        inform_error = false;
    }
    if (inform_error) return;        

    OnCompile(event);
    if (inform_error) return;    

    OnOutput(MESSAGES_CREATINGZBLORB+_T("\n"));

    wxString wpath=wxPathOnly(idFile);
    wxString mpath = wxGetCwd();
    wxSetWorkingDirectory(wpath);

    comando =  _T("\"")+blc +_T("\" \"")+idFile+_T(".zblc\" \"")+idFile+_T(".zblorb\"");

    //wxString comando =  blc +" "+blcFile+" "+blbFile;
    if ( wxExecute(comando, output) != 0 )
    {
        OnOutput(comando);
        OnOutput(_T("\n"));
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            OnOutput(output[n].c_str());
            OnOutput(_T("\n"));
        }
        OnOutput(ERRORS_ERROR+_T("\n"));
        inform_error = true;
    }
    else
    {
        OnOutput(comando);
        OnOutput(_T("\n"));
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            OnOutput(output[n].c_str());
            OnOutput(_T("\n"));
        }
        OnOutput(_T("Ok.\n"));
        inform_error = false;
    }
    wxSetWorkingDirectory(mpath);
}

// Run ZBLB
void MyFrame::OnRunZBlb (wxCommandEvent &event) {
    if (auinotebook->GetPageCount()==0){ return; }
    OnClear();
    OnOutput(MESSAGES_RUNZBLORB+_T("\n"));
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());

    // Running the MAIN FILE
    wxString nome;
    if (mainFile==_T("")){
        nome = e->GetFilename();
    }
    else{
        OnOutput(MESSAGES_USINGMAINFILE+_T(" ")+mainFile+_T("\n"));
        nome = mainFile;
    }
    nome.Replace(_T(".inf"), _T(".zblorb"), true);
    wxString comando =  pConfig->Read(_T("ZCODEINTERPRETER"), _T("")) +_T(" ")+_T("\"")+nome+_T("\"");
    OnOutput(comando+_T("\n"));
    wxArrayString output;
    wxExecute(comando);
}

// Run zcode
void MyFrame::OnRunZcode (wxCommandEvent &event) {
    if (auinotebook->GetPageCount()==0){ return; }
    OnClear();
    OnOutput(MESSAGES_RUNZCODE+_T("\n"));
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    wxString nome = e->GetFilename();
    if (mainFile==_T("")) {
        nome = e->GetFilename();
    } else {
        OnOutput(MESSAGES_USINGMAINFILE+_T(" ")+mainFile+_T("\n"));
        nome = mainFile;
    }
    nome.Replace(_T(".inf"), zcodeversion, true);    
    wxString comando =  pConfig->Read(_T("ZCODEINTERPRETER"), _T("")) +_T(" ")+_T("\"")+nome+_T("\"");
    OnOutput(comando+_T("\n"));
    wxArrayString output; 
    wxExecute(comando);
}

// Compile
void MyFrame::OnCompile (wxCommandEvent &event) {
    // Se non ho finetre aperte esco
    if (auinotebook->GetPageCount()==0){ return; }

    // Azzero la console
    if (event.GetId() != ID_CreateZBlb) OnClear();

    // AS: pulisco la console
    OnClear();
        
    // PL: Salvo tutto
    OnOutput(MESSAGES_SAVINGALL+_T("\n"));
    wxCommandEvent subev;
    OnSaveAll(subev);
        
    OnOutput(MESSAGES_COMPILEZCODE+_T("\n"));
    wxArrayString output;
    
    // Recupero il nome del file da compilare
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    wxString nome,zcode;    
    if (mainFile==_T("")){
        nome = e->GetFilename();
        zcode = e->GetFilename();
    }
    else{
        OnOutput(MESSAGES_USINGMAINFILE+_T(" ")+mainFile+_T("\n"));        
        nome = mainFile;
        zcode = mainFile;        
    }
    zcode.Replace(_T(".inf"), zcodeversion, true);
    //wxString comando =  informCompiler+_T(" ")+zcodeswitch+_T(" ")+libraryPath+_T(" \"")+nome+_T("\" ")+_T(" \"")+zcode+_T("\" ");
    wxString comando =  informCompiler+_T(" ")+zcodeswitch+_T(" ")+libraryPath+_T(" \"")+nome+_T("\" ")+_T(" \"")+zcode+_T("\"");

    
    //if ( wxExecute(_T("./inform.exe twocol.inf"), output) != 0 )
    if ( wxExecute(comando, output) != 0 )    
    {
        OnOutput(comando);
        OnOutput(_T("\n"));
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            OnOutput(output[n].c_str());
            OnOutput(_T("\n"));
        }
        OnOutput(ERRORS_ERROR+_T(".\n"));
        inform_error = true;
    }
    else
    {
        OnOutput(comando);
        OnOutput(_T("\n"));        
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            OnOutput(output[n].c_str());
            OnOutput(_T("\n"));
        }
        OnOutput(_T("Ok.\n"));
        inform_error = false;
    }
}

// EVENTI SULL'ALBERO (TREE)
void MyFrame::OnSelChanged(wxTreeEvent &event)
{
    // Recupero il numero della posizione caret
    wxTreeItemId itemId = event.GetItem();
    MyTreeItemData *item = (MyTreeItemData *)tree->GetItemData(itemId);

    if ( item != NULL )
    {
        // Recupero l'edit corrente
        Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
        int start  = item->GetPosition();
        int end  = item->GetPosition()+item->GetLength();
        int linea = e->LineFromPosition(start);        
        e->GotoLine(linea);        
        e->SetSelection(start,end);        
        //OnOutput(wxString::Format("%d", item->GetPosition())+"\n");        
        //OnOutput(wxString::Format("%d", item->GetPosition())
        //+" "+"start["+wxString::Format("%d", start)+"]"
        //+" "+"end["+wxString::Format("%d", end)+"]\n");
    }
}

// AS: funzione che wrappa la appendText per il problema del readonly
void MyFrame::OnClear(){
    console->SetReadOnly(false);
    console->SetText(_T(""));
    console->SetReadOnly(true);    
}

// AS: funzione che wrappa la appendText per il problema del readonly
void MyFrame::OnOutput(wxString string) {
    console->SetReadOnly(false);    
    console->AppendText(string);
    console->SetReadOnly(true);    
}

// AS
void MyFrame::OnConsoleClicked(wxStyledTextEvent &event) {
    if (event.GetEventObject() == console) {
        // Recupero la riga selezionata
        int posizione = console->GetCurrentPos();       
        wxString risultato = console->GetCurLine(&posizione);
        // Controllo solo se contiene la stringa ERROR
        if (risultato.Contains(_T("Error")) || risultato.Contains(_T("Warning"))) {
            wxString file = risultato.Mid(0,risultato.Find('('));
            wxString riga = risultato.Mid(risultato.Find('(')+1,risultato.Find(')')-risultato.Find('(')-1);
            console->SetSelectionStart(0);
            console->SetSelectionEnd(0);
            console->Clear();
            
            // Se il file non � aperto, lo apro        
            if (!checkOpenFile(file)){
                wxString nome = file.Mid(file.Find('\\',true)+1,file.Length());
                LoadFile(file, nome);            
            }
            
            int pannelli = auinotebook->GetPageCount();
            for (int i=0; i<=pannelli-1 ;i++) {
                Edit* e = (Edit*) auinotebook->GetPage(i);
                wxString nome = e->GetFilename();
                if (file.Cmp(nome)==0){
                    e->GotoLine(wxAtoi(riga)-1);
                    int lineStart = e->PositionFromLine (e->GetCurrentLine());
                    int lineEnd = e->PositionFromLine (e->GetCurrentLine() + 1);
                    e->SetSelection (lineStart, lineEnd);
                    auinotebook->SetSelection(i);
                }
            }            
        }
    }   
}


// EVENTI SU NOTEBOOK
void MyFrame::OnNotebookPageChanged(wxAuiNotebookEvent& evt)
{
    OnUpdateTree();
}

void MyFrame::OnNotebookPageClose(wxAuiNotebookEvent& evt)
{   
    wxAuiNotebook* ctrl = (wxAuiNotebook*)evt.GetEventObject();
    if (ctrl->GetPage(evt.GetSelection())->IsKindOf(CLASSINFO(Edit)))
    {
        Edit* e = (Edit*) ctrl->GetPage(evt.GetSelection());
        wxString titolo = ctrl->GetPageText(evt.GetSelection());
        if (e->Modified()) {
            int risposta = wxMessageBox (MESSAGES_TEXTNOTSAVED, titolo,
            wxYES_NO | wxCANCEL | wxICON_QUESTION);

            if (risposta == wxYES) {
                e->SaveFile();
                if (e->Modified()) {
                    wxMessageBox (ERRORS_SAVEERROR, ERRORS_CLOSEABORT,
                    wxOK | wxICON_EXCLAMATION);
                }
            }
            else if (risposta == wxCANCEL){
                evt.Veto();
            }
        }
    }
    tree->DeleteAllItems(); 
    OnClear();     
}

void MyFrame::OnExit (wxCommandEvent &event) {
    int pannelli = auinotebook->GetPageCount();
    for (int i=0; i<=pannelli ;i++){
        Edit* e = (Edit*) auinotebook->GetPage(i);
        wxString titolo = auinotebook->GetPageText(i);
        if (e) {
            if (e->Modified()) {
                int risposta = wxMessageBox (MESSAGES_TEXTNOTSAVED, titolo,
                    wxYES_NO | wxCANCEL | wxICON_QUESTION);
                if (risposta == wxYES) {
                    e->SaveFile();
                    if (e->Modified()) {
                        wxMessageBox (ERRORS_SAVEERROR, ERRORS_CLOSEABORT,
                        wxOK | wxICON_EXCLAMATION);
                        continue;
                    }
                }
                else if (risposta == wxCANCEL){
                    return;
                }
            }
        }
    }
    Destroy();
}

void MyFrame::OnClose (wxCloseEvent &event) {
    int pannelli = auinotebook->GetPageCount();
    for (int i=0; i<=pannelli ;i++){
        Edit* e = (Edit*) auinotebook->GetPage(i);
        wxString titolo = auinotebook->GetPageText(i);
        if (e) {
            if (e->Modified()) {
                int risposta = wxMessageBox (MESSAGES_TEXTNOTSAVED, titolo,
                    wxYES_NO | wxCANCEL | wxICON_QUESTION);
                if (risposta == wxYES) {
                    e->SaveFile();
                    if (e->Modified()) {
                        wxMessageBox (ERRORS_SAVEERROR, ERRORS_CLOSEABORT,
                        wxOK | wxICON_EXCLAMATION);
                        continue;
                    }
                }
                else if (risposta == wxCANCEL){
                    return;
                }
            }
        }
    }
    OnUpdateTree();   
    Destroy();    
}
 
// Tutti gli eventi di EDIT vengono gestiti dal componente Stc 
void MyFrame::OnEdit (wxCommandEvent &event) {
    
    int id = event.GetId();
//    wxMessageBox (_("Sono qui!"), _("Close abort"),  wxOK | wxICON_EXCLAMATION);
    switch (id)
    {
        case myID_LINENUMBER: showLineNumber=event.IsChecked(); break;
        case myID_WRAPMODEON: wrapMode=event.IsChecked(); break;
        case myID_AUTOCOMPON: autoCompleteSwitch=event.IsChecked(); break;
        case myID_HOTKEYSON:  hotkeysSwitch=event.IsChecked(); break;
    }    
    
    int pannelli = auinotebook->GetPageCount();
    for (int i=0; i<=pannelli ;i++){
        Edit* e = (Edit*) auinotebook->GetPage(i);
        if (e) {
            e->ProcessEvent (event);
        }
    }       
}
 
bool MyFrame::checkOpenFile(wxString path)
{
    bool result = false;
    int pannelli = auinotebook->GetPageCount();
    for (int i=0; i<=pannelli-1 ;i++) {
        Edit* e = (Edit*) auinotebook->GetPage(i);
        wxString nome = e->GetFilename();
        if (path.Cmp(nome)==0){
            result = true;
        }        
    }    
    return result;
} 

static int CompareStrings(const wxString& first, const wxString& second)
{
    return first.CmpNoCase(second);
}

void MyFrame::setNewStc(Edit* stc) {
    // Impostazioni del nuovo stc : PL
    if (showLineNumber){
        stc->SetMarginWidth (0, stc->TextWidth (wxSTC_STYLE_LINENUMBER, _T("_999999")));
    }
    else{
        stc->SetMarginWidth (0, 0);
    }
    if (wrapMode){
        stc->SetWrapMode (wxSTC_WRAP_WORD);
    }
    else{
        stc->SetWrapMode (wxSTC_WRAP_NONE);
    }

    stc->SetCaretLineVisible(true);
    //stc->SetCaretLineBackground(wxColour(152,248,248,wxALPHA_OPAQUE));
    stc->AutoCompSetIgnoreCase(true);
    stc->AutoCompSetAutoHide(true);
    //stc->CallTipSetForegroundHighlight(wxColour(52,248,248));

    //wxString languagename = stc->QueryLanguage();
    if (stc->QueryLanguage() != _T("INFORM")) {
        stc->ClearHotkeys();
        stc->SetAutoComplete(false, false);
        stc->SetHotkeys(false, false);   
        return;     
    }
    bool bCont;
    long dummy;
    wxString str, s;
    // Recupero le hotkey
    /*
    stc->ClearHotkeys();
    pConfig->SetPath(_T("/HOTKEYS"));
    bCont = pConfig->GetFirstEntry(str, dummy);
    while(bCont){
        s = pConfig->Read(str,_T(""));
        stc->AddHotkey(s);
        //OnOutput(_T(str)+" = "+_T(s)+" ;  ");
        bCont = pConfig->GetNextEntry(str, dummy);
    } 
    */  
    //wxMessageBox (_("Sono qui!"), _("Close abort"),  wxOK | wxICON_EXCLAMATION); 
    
    // Recupero tutte le wordlist per AUTOCOMPLETION dal file di configurazione
    // Recupero le liste separatamente, aggiungendo le parole sia alle varie
    // liste per la syntax highlighting che all'arraty wlarray
    wxArrayString wlarray;
    wxString statlist = _T("");
    wxString direclist = _T("");
    wxString otherlist = _T("");
    wxString informatelist = _T("");
    wxString custom1list = _T("");
    wxString custom2list = _T("");
    wxString custom3list = _T("");
    
    if (useStatements){
        pConfig->SetPath(_T("/STATEMENTS"));        
        bCont = pConfig->GetFirstEntry(str, dummy);
        while(bCont){
            s = pConfig->Read(str,_T(""));
            statlist = statlist + s + _T(" ");
            wlarray.Add(s);
            bCont = pConfig->GetNextEntry(str, dummy);
        }
    }
    
    if (useDirectives){
        pConfig->SetPath(_T("/DIRECTIVES"));        
        bCont = pConfig->GetFirstEntry(str, dummy);
        while(bCont){
            s = pConfig->Read(str,_T(""));
            direclist = direclist + s + _T(" ");
            wlarray.Add(s);
            bCont = pConfig->GetNextEntry(str, dummy);
        }
    }
    
    if (useOtherkeywords){
        pConfig->SetPath(_T("/OTHERKEYWORDS"));
        bCont = pConfig->GetFirstEntry(str, dummy);
        while(bCont){
            s = pConfig->Read(str,_T(""));
            otherlist = otherlist + s + _T(" ");
            wlarray.Add(s);
            bCont = pConfig->GetNextEntry(str, dummy);
        }    
    }
    
    if (useInformate){
        pConfig->SetPath(_T("/INFORMATE"));
        bCont = pConfig->GetFirstEntry(str, dummy);
        while(bCont){
            s = pConfig->Read(str,_T(""));
            informatelist = informatelist + s + _T(" ");
            wlarray.Add(s);
            bCont = pConfig->GetNextEntry(str, dummy);
        }    
    }
    
    if (useCustom1){
        pConfig->SetPath(_T("/CUSTOM1"));
        bCont = pConfig->GetFirstEntry(str, dummy);
        while(bCont){
            s = pConfig->Read(str,_T(""));
            custom1list = custom1list + s + _T(" ");
            wlarray.Add(s);
            bCont = pConfig->GetNextEntry(str, dummy);
        }    
    }
    if (useCustom2){
        pConfig->SetPath(_T("/CUSTOM2"));
        bCont = pConfig->GetFirstEntry(str, dummy);
        while(bCont){
            s = pConfig->Read(str,_T(""));
            custom2list = custom2list + s + _T(" ");
            wlarray.Add(s);
            bCont = pConfig->GetNextEntry(str, dummy);
        }    
    }
    if (useCustom3){
        pConfig->SetPath(_T("/CUSTOM3"));
        bCont = pConfig->GetFirstEntry(str, dummy);
        while(bCont){
            s = pConfig->Read(str,_T(""));
            custom3list = custom3list + s + _T(" ");
            wlarray.Add(s);
            bCont = pConfig->GetNextEntry(str, dummy);
        }    
    }
    
    
    
    pConfig->SetPath(_T("/"));
    //wxString wordlist = statlist+direclist+otherlist;
    // Aggiungo la roba del progetto
    if ( mainFile != _T("") ) {
        for (size_t i = 0; i<projclasses.GetCount(); i++) {
            direclist = direclist + projclasses[i] + _T(" ");
            wlarray.Add(projclasses[i]);
        }
        for (size_t i = 0; i<projkeywords.GetCount(); i++) {
            otherlist = otherlist + projkeywords[i] + _T(" ");
            wlarray.Add(projkeywords[i]);
        }            
    }
    //Ordino l'array e lo copio nella lista per l'autocompletion
    wlarray.Sort(CompareStrings);
    wxString wordlist = _T("");
    for (size_t i = 0; i<wlarray.GetCount(); i++) wordlist = wordlist + wlarray[i].MakeLower() + _T(" ");
    wlarray.Clear();
    stc->SetWordlist(wordlist);
    stc->SetKeyWords (mySTC_TYPE_DEFAULT, statlist + direclist + otherlist + informatelist + custom1list + custom2list + custom3list);
    //stc->SetKeyWords (1, direclist);
    //stc->SetKeyWords (2, otherlist);   
    stc->SetAutoCompleteNumber(autoCompleteNumber);
    stc->SetAutoComplete(autoCompleteSwitch, true);   
    stc->SetHotkeys(hotkeysSwitch, true);
    stc->SetTabWidth(tabSize);

    /*OnClear();
    OnOutput(wordlist);
    wxMessageBox (_("Sono qui!"), _("Close abort"),  wxOK | wxICON_EXCLAMATION); */
    
}
    
 
void MyFrame::LoadFile(wxString path, wxString name)
{
    if (checkOpenFile(path)) return;
    Edit* stc = new Edit( auinotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER, path, this);
    stc->LoadFile (path);
    
    setNewStc(stc);
   
    OnClear();

    wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16));
    auinotebook->AddPage(stc  , name, true , page_bmp);
    OnUpdateTree();
}
 
void MyFrame::OnLoadFile(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog* fd = new wxFileDialog(this, _T("Open File"),_T(""),_T(""),_T("Inform Source Files (*.inf;*.h)|*.inf;*.h|All Files (*.*)|*.*"),
    wxFD_DEFAULT_STYLE,wxDefaultPosition,wxDefaultSize,_T("filedlg"));
    wxString path,name;
    if (fd->ShowModal() == wxID_OK ){
        path = fd->GetPath();
        name = fd->GetFilename();
        
        // Check if file is already opened
        if (checkOpenFile(path)){
           wxMessageBox (MESSAGES_FILEOPENED,MESSAGES_WARNING, wxOK | wxICON_WARNING);    
           return;
        }
        
        Edit* stc = new Edit( auinotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER, path, this);
        stc->LoadFile (path);
        
        setNewStc(stc);
        
        OnClear();
        
        wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16));
        auinotebook->AddPage(stc  , name, true , page_bmp);
        //auinotebook->SetSelection(auinotebook->GetSelection()+1);
        OnUpdateTree();        
    }   
} 

// MENU PROJECT

void MyFrame::OnOpenProject(wxCommandEvent& WXUNUSED(event)) {
    wxFileDialog* fd = new wxFileDialog(this, _T("Open Wide Project"),_T(""),_T(""),_T("Wide Project Files (*.wpf)|*.wpf|All Files (*.*)|*.*"),
    wxFD_DEFAULT_STYLE,wxDefaultPosition,wxDefaultSize,_T("filedlg"));    
    if (fd->ShowModal() == wxID_OK ){
        wxString path = fd->GetPath();
        wxString name = fd->GetFilename();        
        OnClear();
        mainFile = _T("");
        projclasses.Empty();
        projkeywords.Empty();
        
        wxFileConfig* projfile = new wxFileConfig(
        _(NOMEAPPLICAZIONE), _(NOMEAPPLICAZIONE),
        path, _T(""), wxCONFIG_USE_RELATIVE_PATH, wxConvUTF8);
        //wxConfigBase::Set(projfile);
        //projfile->SetPath(_T("/")); 

        wxString s, str;
        bool bCont; long dummy;
        int zc = 0;
        zc = projfile->Read(_T("ZCODEVERSION"), 1l);
        switch (zc) {
          case 8:
            zcodeversion=_T(".z8");
            zcodeswitch=_T("-v8");
            zcodemenu->Check(ID_ZcodeVersion8, true);
            break;
          case 5:
            zcodeversion=_T(".z5");
            zcodeswitch=_T("-v5");
            zcodemenu->Check(ID_ZcodeVersion5, true);
            break;
        }        
        projfile->SetPath(_T("/CLASSES"));
        bCont = projfile->GetFirstEntry(str, dummy);
        while(bCont){
          s = projfile->Read(str,_T(""));
          projclasses.Add(s);
          bCont = projfile->GetNextEntry(str, dummy);
        }        
        projfile->SetPath(_T("/KEYWORDS"));
        bCont = projfile->GetFirstEntry(str, dummy);
        while(bCont){
          s = projfile->Read(str,_T(""));
          projkeywords.Add(s);
          bCont = projfile->GetNextEntry(str, dummy);
        }        
        projfile->SetPath(_T("/FILES"));
        bCont = projfile->GetFirstEntry(str, dummy);
        while(bCont){
          s = projfile->Read(str,_T(""));
          str = path.Mid(0,path.Find('\\',true)+1) + s;
          LoadFile(str,s);
          if (mainFile == _T("")) {
                mainFile = str;
                OnOutput(MESSAGES_USINGMAINFILE+_T(" [")+mainFile+_T("] "));
          }
          bCont = projfile->GetNextEntry(str, dummy);
        }
        //wxMessageBox (_(path), _("Close abort"),  wxOK | wxICON_EXCLAMATION);

    }    
    //wxConfigBase::Set(pConfig);    
}
  


void MyFrame::OnSaveProject(wxCommandEvent& WXUNUSED(event))
{
}

void MyFrame::OnCloseProject(wxCommandEvent& WXUNUSED(event))
{
    projclasses.Empty();
    projkeywords.Empty();
    if (mainFile!=_T("")){
        wxMessageBox (mainFile, MESSAGES_REMOVEMAINFILE,  wxOK | wxICON_INFORMATION);            
        mainFile=_T("");
        OnClear();
        OnOutput(MESSAGES_MAINFILEREMOVED);        
    }
    else{
        wxMessageBox (MESSAGES_NOPROJECTOPENED,MESSAGES_WARNING, wxOK | wxICON_WARNING);    
    }
}


void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    
    wxAboutDialogInfo info;
    info.SetName(_(NOMEAPPLICAZIONE));
    info.SetVersion(_(VERSIONE));
    info.SetDescription(_(DESCRIZIONE));
    info.SetCopyright(_T("(C) 2008 - 2013 silver.slade@tiscali.it"));
    info.SetWebSite(_T("http://wide.berlios.de/"), _T("Home page"));
    info.AddDeveloper(_T("Alessandro Schillaci"));
    info.AddDeveloper(_T("Paolo Lucchesi"));
    info.AddTranslator(_T("Urbatain (Spanish)"));
    info.AddTranslator(_T("Eric Forgeot (French)"));
    info.AddTranslator(_T("Christof Menear (German)"));
    info.SetDescription(wxString::FromAscii(
    "Wide is a win32 Editor for Inform6:\n"
    "is an Integrated Development Environment (IDE), written entirely\n"
    "in C++/WxWidgets, for the creation of text adventures based on \n"
    "Graham Nelson's Inform6 Language. With Wide it's possible to edit,\n"
    "compile and run a text adventure in the z-code and glulx formats.\n\n"
    "- Written in C++ (using the wxWidgets Library)\n"
    "- Wide runs on Win32 / Linux systems\n"
    "- Wide is localized in spanish, english, italian, german and french\n"
    "- Auto-completion window, in edit mode\n"
    "- Wide is just an exe file: no DLL, no virtual machines: it's a stand alone application.\n"
    "- Wrap lines, show number of lines, code folding\n"
    "- Inform/Glulx support: wide supports zcode (5 and 8 version), ulx, blb, bres\n"
    "- Objects tree and Syntax highlight is customizable\n"
    "- Wide is portable: it can be started from an USB key/pen\n"));
    info.SetLicense(wxString::FromAscii(
"\nGNU GPL LICENSE\n"
"\n"
"This program is free software; you can redistribute it and/or modify it\n"
"under the terms of the GNU General Public License as published by the  \n"
"Free Software Foundation; either version 2 of the License, or (at your \n"
"option) any later version. This program is distributed in the hope that\n"
"it will be useful,  but WITHOUT ANY WARRANTY; without even the implied \n"
"warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. \n"
"See the GNU General Public License for more details. \n\nYou should have"
"received a copy of the GNU General Public License \nalong with this"
"program.\nIf not, see <http://www.gnu.org/licenses/>."
    ));
    //info.SetIcon(logo_xpm);
    wxAboutBox(info);    
}
 
// Funzione che aggiunge un nodo alla root con i valori relativi
void MyFrame::OnUpdateTreeRegularExpression(wxString text, wxTreeItemId root, wxString nome, wxString pattern_global, bool keepquote){
    int contatore=0;    
    wxRegEx re;
    if (!re.Compile(pattern_global, wxRE_ICASE|wxRE_NEWLINE) ){
        OnOutput(ERRORS_RE);
        return;
    }
    wxArrayTreeItemIds items;
    items.Add(tree->AppendItem(root, nome, 0));
    
    while ( re.Matches(text) ){
        size_t start, len, n=0;
        int i=0;
        while (re.GetMatch(&start, &len, n)){
            n+=1;
        }
        if (text[start-1]=='\"' && keepquote) {start--; len++; }
        wxTreeItemId id = items.Item(i);
        tree->AppendItem(id, text.Mid(start,len),1,-1, new MyTreeItemData(start+contatore,len));
        i++;
        text = text.Mid(start+len,text.Len());
        contatore=contatore + start+len;
    }        
}
 
void MyFrame::OnUpdateTree()
{
    if (auinotebook->GetPageCount()==0){
        tree->DeleteAllItems(); 
        return;
    }
    
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    
    wxTreeItemId wroot = tree->GetRootItem();
    wxTreeItemIdValue ck, ck2;
    wxTreeItemId cid, cen;
    wxString cname;
    wxArrayString memo;
    cid = tree->GetFirstChild(wroot, ck);
    while (cid.IsOk()) {
        if (tree->ItemHasChildren(cid)) {
          if (tree->IsExpanded(cid)) cname = _T("+") + tree->GetItemText(cid);
           else                      cname = _T("-") + tree->GetItemText(cid);
          memo.Add(cname);
    }
        //OnOutput(cname);
        cid = tree->GetNextChild(wroot, ck);
    }
    cid = tree->GetFirstVisibleItem();
    wxString firstvis = tree->GetItemText(cid);
    
    wxString text = e->GetText();
    tree->DeleteAllItems();    
    wxArrayTreeItemIds items;
    wxTreeItemId root = tree->AddRoot(wxT("Object Tree"), 0);


    if (showObjects) OnUpdateTreeRegularExpression(text, root, _T("Object"), _T("\n+[ \t\f\v]*Object[ \t\n\r\f\v]+(->[ \t\n\r\f\v]+)*\"?([A-Za-z0-9_]+)\"?"),true);
    if (showProject) {
        for (size_t i = 0; i<projclasses.GetCount(); i++) {
            wxString regexp = _T("\n+[ \t\f\v]*")+projclasses[i]+_T("[ \t\n\r\f\v]+(->[ \t\n\r\f\v]+)*\"?([A-Za-z0-9_]+\"?)");
            OnUpdateTreeRegularExpression(text, root, projclasses[i], regexp, true);
        }        
    }
    if (showGlobals) OnUpdateTreeRegularExpression(text, root, _T("Global"), _T("\n+[ \t\f\v]*Global[ \t\n\r\f\v]+([A-Za-z0-9_]+)"), false);
    if (showConstants) OnUpdateTreeRegularExpression(text, root, _T("Constant"), _T("\n+[ \t\f\v]*Constant[ \t\n\r\f\v]+([A-Za-z0-9_]+)"), false);
    if (showFunctions) OnUpdateTreeRegularExpression(text, root, _T("Function"), _T("\n+[ \t\n\r\f\v]*\\[[ \t\n\r\f\v]*([A-Za-z0-9_]+).*;"), false);
    if (showClasses) OnUpdateTreeRegularExpression(text, root, _T("Class"), _T("\n+[ \t\f\v]*Class[ \t\n\r\f\v]+([A-Za-z0-9_]+)"), false);
    //if (showIncludes) OnUpdateTreeRegularExpression(text, root, "Include", "\n+[ \t\f\v]*Include[ \t\f\v]*\"([A-Za-z0-9_]+)(\x2eh)?\"", false);
    if (showIncludes) OnUpdateTreeRegularExpression(text, root, _T("Include"), _T("\n+[ \t\f\v]*Include[ \t\f\v]*\"([A-Za-z0-9_]+)"), false);
    if (showVerbs) OnUpdateTreeRegularExpression(text, root, _T("Verb"), _T("\n+[ \t\f\v]*Verb[ \t\f\v]+\'([A-Za-z0-9_]+)'"), false);

    if (expandAllNodes) {
        tree->ExpandAll();                
    }
    else{
        tree->Expand(root);     
    }
    
    wroot = tree->GetRootItem();
    cid = tree->GetFirstChild(wroot, ck);
    while (cid.IsOk()) {
        cname = tree->GetItemText(cid);
        for (size_t i = 0; i<memo.GetCount(); i++) {
            if (memo[i] == _T("+") + cname) { tree->Expand(cid); break; }
            if (memo[i] == _T("-") + cname) { tree->Collapse(cid); break; }
        }
        cid = tree->GetNextChild(wroot, ck);
    }
    
    // Scroll all'inizio dell'albero
    tree->ScrollTo(root);
    cid = tree->GetFirstChild(wroot, ck);
    bool brf = false;
    while (cid.IsOk()) {
        if ((tree->GetItemText(cid)) == firstvis) { tree->ScrollTo(cid); break; }
        cen = tree->GetFirstChild(cid, ck2);
        while (cen.IsOk()) {
            if ((tree->GetItemText(cen)) == firstvis) { tree->ScrollTo(cen); brf = true; break; }
            cen = tree->GetNextChild(cid, ck2);
        }
        if (brf) break;
        cid = tree->GetNextChild(wroot, ck);
    }
    tree->SetScrollPos(wxHORIZONTAL, 0, true);
    
}
 
 
  
 

// FUNZIONI CREAZIONE CONTROLLI

// NOTEBOOK
wxAuiNotebook* MyFrame::CreateNotebook()
{
   // create the notebook off-window to avoid flicker
   wxSize client_size = GetClientSize();
   
   wxAuiNotebook* ctrl = new wxAuiNotebook(this, wxID_ANY,
                                    wxPoint(client_size.x, client_size.y),
                                    wxSize(430,200),
                                    wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER);
   return ctrl;
}


// MENUBAR |--------------------------------------------------------------------
wxMenuBar* MyFrame::CreateMenuBar()
{
    wxMenuBar* mb = new wxMenuBar;

    // FILE MENU ----------------------------------------------------------------------------
    wxMenu* file = new wxMenu;    
    // FILE::NEW
    wxMenuItem *new_file = new wxMenuItem(file, ID_NewFile, _T("&")+MENU_FILE_NEWFILE+_T("\tCtrl+N"));
    new_file->SetBitmap(new_xpm);
    file->Append(new_file);    
    
    // FILE::LOAD
    wxMenuItem *load_file = new wxMenuItem(file, ID_LoadFile, _T("&")+MENU_FILE_OPENFILE+_T("\tCtrl+O"));
    load_file->SetBitmap(fileopen_xpm);
    file->Append(load_file);

    // FILE::SAVE
    wxMenuItem *save_file = new wxMenuItem(file, ID_Save_File, _T("&")+MENU_FILE_SAVEFILE+_T("\tCtrl+S"));
    save_file->SetBitmap(filesave_xpm);
    file->Append(save_file);
    
    //FILE::SAVEALL   //PL
    wxMenuItem *save_all = new wxMenuItem(file, ID_Save_All, _T("&")+MENU_FILE_SAVEALL+_T("\tCtrl+Shift+S"));
    save_all->SetBitmap(filesaveall_xpm);
    file->Append(save_all);
    
    file->AppendSeparator();

    //FILE::NEXTPAGE   //PL
    wxMenuItem *next_page = new wxMenuItem(file, ID_NextPage, _T("&")+MENU_FILE_NEXTTAB+_T("\tCtrl+F6"));
    next_page->SetBitmap(forward_xpm);
    file->Append(next_page);
    
    //FILE::PREVIOUSPAGE   //PL
    wxMenuItem *previous_page = new wxMenuItem(file, ID_PreviousPage, _T("&")+MENU_FILE_PREVIOUSTAB+_T("\tCtrl+F5"));
    previous_page->SetBitmap(back_xpm);
    file->Append(previous_page);

    file->AppendSeparator();
    
    wxMenuItem *quit = new wxMenuItem(file, ID_Exit, _T("&")+MENU_FILE_QUIT+_T("\tCtrl+Q"));
    quit->SetBitmap(quit_xpm);
    file->Append(quit);

    // EDIT MENU -------------------------------------------------------------------------
    wxMenu* edit = new wxMenu;
    // EDIT::COPY
    wxMenuItem *copy = new wxMenuItem(edit, wxID_COPY, _T("&")+MENU_EDIT_COPY+_T("\tCtrl+C"));
    copy->SetBitmap(copy_xpm);
    edit->Append(copy);

    // EDIT::CUT
    wxMenuItem *cut = new wxMenuItem(edit, wxID_CUT, _T("&")+MENU_EDIT_CUT+_T("\tCtrl+X"));
    cut->SetBitmap(cut_xpm);
    edit->Append(cut);
    
    // EDIT::PASTE
    wxMenuItem *paste = new wxMenuItem(edit, wxID_PASTE, _T("&")+MENU_EDIT_PASTE+_T("\tCtrl+V"));
    paste->SetBitmap(paste_xpm);
    edit->Append(paste);    
    edit->AppendSeparator();

    edit->Append(wxID_UNDO, _T("&")+MENU_EDIT_UNDO+_T("\tCtrl+Z"));
    edit->Append(wxID_REDO, _T("&")+MENU_EDIT_REDO+_T("\tCtrl+Y"));
    edit->AppendSeparator();
    
    // EDIT::INDENT
    wxMenuItem *indent_r = new wxMenuItem(edit, myID_INDENTINC, _T("&")+MENU_EDIT_INDENT+_T("\tTab"));
    //indent_r->SetBitmap(forward_xpm);
    edit->Append(indent_r);

    // EDIT::UNINDENT
    wxMenuItem *indent_l = new wxMenuItem(edit, myID_INDENTRED, _T("&")+MENU_EDIT_UNINDENT+_T("\tShift+Tab"));
    //indent_l->SetBitmap(back_xpm);
    edit->Append(indent_l);
    edit->AppendSeparator();

    // EDIT::COMMENT
    wxMenuItem *comment = new wxMenuItem(edit, ID_Comment, _T("&")+MENU_EDIT_COMMENT+_T("\tCtrl+Shift+C"));
    //indent_l->SetBitmap(back_xpm);
    edit->Append(comment);

    // EDIT::UNCOMMENT
    wxMenuItem *uncomment = new wxMenuItem(edit, ID_Uncomment, _T("&")+MENU_EDIT_UNCOMMENT+_T("\tCtrl+Shift+U"));
    //indent_l->SetBitmap(back_xpm);
    edit->Append(uncomment);

    // SEARCH MENU                     
    wxMenu* search = new wxMenu;
    search->Append (ID_Find, _T("&")+MENU_SEARCH_FIND+_T("\tF3"));
    search->Append (ID_FindBack, _T("&")+MENU_SEARCH_FIND_BACKWARDS+_T("\tShift+F3"));   //PL
    search->Append (ID_FindReplace, MENU_SEARCH_FIND_REPLACE+_T("\tCtrl+F3"));
    search->Append (ID_FindObjectLocal, _T("&")+MENU_SEARCH_FIND_OBJECT+_T("\tF4"));
    search->Append (ID_FindObjectGlobal, _T("&")+MENU_SEARCH_FIND_OBJECT_IN_FILE+_T("\tShift+F4"));
    search->AppendSeparator();
    search->Append(ID_GotoLine, _T("&")+MENU_SEARCH_FIND_GOTO_LINE+_T("\tCtrl+L"));
    search->AppendSeparator();
    search->Append(ID_NextMarker, _T("&")+MENU_SEARCH_FIND_NEXT_MARKER+_T("\tF2"));
    wxMenuItem *marker = new wxMenuItem(search, ID_ToggleMarker, _T("&")+MENU_SEARCH_FIND_TOGGLE_MARKER+_T("\tCtrl+F2"));
    
    marker->SetBitmap(addbookm_xpm);
    search->Append(marker);        
    search->Append(ID_ResetMarkers, _T("&")+MENU_SEARCH_FIND_RESET_MARKERS+_T("\tCtrl+Shift+F2"));

    // PROJECT MENU
    wxMenu* project = new wxMenu;
    project->Append (ID_OpenProject, _T("&")+MENU_PROJECT_OPEN+_T("\tCtrl+P"));
    project->Append (ID_SaveProject, _T("&")+MENU_PROJECT_SAVE);
    project->Append (ID_CloseProject, _T("&")+MENU_PROJECT_CLOSE);
       
    // ZCODE MENU
    wxMenu* zcode = new wxMenu;
    zcode->Append (ID_Compile, _T("&")+MENU_ZCODE_COMPILEZCODE+_T("\tF8"));
    zcode->Append (ID_RunZcode, _T("&")+MENU_ZCODE_RUNZCODE+_T("\tCtrl+F8"));
    zcode->AppendSeparator();
    zcode->AppendRadioItem (ID_ZcodeVersion5, MENU_ZCODE_ZCODE5);
    zcode->AppendRadioItem (ID_ZcodeVersion8, MENU_ZCODE_ZCODE8);
    zcode->AppendSeparator();
    zcode->Append (ID_CreateZBlb, MENU_ZCODE_COMPILEZCODE_BLORB+_T("\tCtrl+Shift+F8"));
    zcode->Append (ID_RunZBlb, MENU_ZCODE_RUNZCODE_BLORB+_T("\tShift+F8"));    
    zcodemenu=zcode;
    
    // GLULX MENU
    wxMenu* glulx = new wxMenu;
    glulx->Append (ID_CompileUlx, _T("&")+MENU_GLULX_COMPILEULX+_T("\tF9"));            
    glulx->Append (ID_RunUlx, _T("&")+MENU_GLULX_RUNULX+_T("\tCtrl+F9"));
    glulx->AppendSeparator();
    glulx->Append (ID_CreateRes, _T("&")+MENU_GLULX_CREATERESOURCES+_T("\tF10"));
    glulx->Append (ID_CreateBlb, _T("&")+MENU_GLULX_CREATEBLORBFILE+_T("\tCtrl+F10"));
    glulx->Append (ID_MakeAllBlb, _T("&")+MENU_GLULX_BUILDALLBLORBFILE+_T("\tF11"));
    glulx->Append (ID_RunBlb, _T("&")+MENU_GLULX_RUNBLORB+_T("\tCtrl+F11"));

    // SYNTAX
    wxMenu* syntaxmenu = new wxMenu;
    syntaxmenu->AppendCheckItem (ID_UseDirectives, _T("DIRECTIVES"));
    syntaxmenu->AppendCheckItem (ID_UseStatements, _T("STATEMENTS"));
    syntaxmenu->AppendCheckItem (ID_UseOtherkeywords, _T("OTHERKEYWORDS"));
    syntaxmenu->AppendCheckItem (ID_UseInformate, _T("INFORMATE"));
    syntaxmenu->AppendCheckItem (ID_UseCustom1, _T("CUSTOM1"));
    syntaxmenu->AppendCheckItem (ID_UseCustom2, _T("CUSTOM2"));
    syntaxmenu->AppendCheckItem (ID_UseCustom3, _T("CUSTOM3"));
    
    syntaxmenu->Check(ID_UseDirectives, useDirectives);
    syntaxmenu->Check(ID_UseStatements, useStatements);
    syntaxmenu->Check(ID_UseOtherkeywords, useOtherkeywords);
    syntaxmenu->Check(ID_UseInformate, useInformate);
    syntaxmenu->Check(ID_UseCustom1, useCustom1);
    syntaxmenu->Check(ID_UseCustom2, useCustom2);
    syntaxmenu->Check(ID_UseCustom3, useCustom3);
    

    // OBJECT TREE MENU
    wxMenu* otree = new wxMenu;
    otree->Append (ID_RefreshTree, MENU_OBJECTTREE_REFRESHTREE+_T("\tF12"));
    otree->AppendCheckItem (ID_ExpandAllAlways, MENU_OBJECTTREE_EXPAND_NODES);
    otree->AppendSeparator();
    otree->AppendCheckItem (ID_ShowObject, MENU_OBJECTTREE_SHOW_OBJECTS);
    otree->AppendCheckItem (ID_ShowProject, MENU_OBJECTTREE_SHOW_PROJECT_DEFS);
    otree->AppendCheckItem (ID_ShowGlobal, MENU_OBJECTTREE_SHOW_GLOBALS);
    otree->AppendCheckItem (ID_ShowConstant, MENU_OBJECTTREE_SHOW_CONSTANTS);
    otree->AppendCheckItem (ID_ShowFunction, MENU_OBJECTTREE_SHOW_FUNCTIONS);
    otree->AppendCheckItem (ID_ShowClass, MENU_OBJECTTREE_SHOW_CLASSES);
    otree->AppendCheckItem (ID_ShowInclude, MENU_OBJECTTREE_SHOW_INCLUDES);
    otree->AppendCheckItem (ID_ShowVerb, MENU_OBJECTTREE_SHOW_VERBS);

    // OBJECT TREE MENU - Load configuration from file
    otree->Check(ID_ExpandAllAlways, (pConfig->Read(_T("OBJECT_TREE_EXPAND_ALL_NODES"), 1l) != 0) );
    otree->Check(ID_ShowObject, (pConfig->Read(_T("OBJECT_TREE_SHOW_OBJECTS"), 1l) != 0) );
    otree->Check(ID_ShowProject, (pConfig->Read(_T("OBJECT_TREE_SHOW_PROJECT"), 1l) != 0) );
    otree->Check(ID_ShowGlobal, (pConfig->Read(_T("OBJECT_TREE_SHOW_GLOBALS"), 1l) != 0) );
    otree->Check(ID_ShowConstant, (pConfig->Read(_T("OBJECT_TREE_SHOW_CONSTANTS"), 1l) != 0) );
    otree->Check(ID_ShowFunction, (pConfig->Read(_T("OBJECT_TREE_SHOW_FUNCTIONS"), 1l) != 0) );
    otree->Check(ID_ShowClass, (pConfig->Read(_T("OBJECT_TREE_SHOW_CLASSES"), 1l) != 0) );
    otree->Check(ID_ShowInclude, (pConfig->Read(_T("OBJECT_TREE_SHOW_INCLUDES"), 1l) != 0) );
    otree->Check(ID_ShowVerb, (pConfig->Read(_T("OBJECT_TREE_SHOW_VERBS"), 1l) != 0) );
    
    // OPTION MENU
    wxMenu* option = new wxMenu;
    option->AppendCheckItem (myID_LINENUMBER, MENU_OPTIONS_SHOWLINENUMBER);
    option->AppendCheckItem (myID_WRAPMODEON, _T("&")+MENU_OPTIONS_WRAPMODE);    
    option->AppendCheckItem (myID_AUTOCOMPON, _T("&")+MENU_OPTIONS_AUTOCOMPLETE);
    option->AppendCheckItem (myID_HOTKEYSON,  _T("&")+MENU_OPTIONS_HOTKEYSREPLACING);
    option->Check(myID_LINENUMBER, showLineNumber);
    option->Check(myID_WRAPMODEON, wrapMode);
    option->Check(myID_AUTOCOMPON, autoCompleteSwitch);
    option->Check(myID_HOTKEYSON,  hotkeysSwitch);
    option->AppendSeparator();
    option->AppendRadioItem (myID_ENGLISH, MENU_LANGUAGE_ENGLISH);
    option->AppendRadioItem (myID_ITALIAN, MENU_LANGUAGE_ITALIAN);
    option->AppendRadioItem (myID_SPANISH, MENU_LANGUAGE_SPANISH);
    option->AppendRadioItem (myID_GERMAN, MENU_LANGUAGE_GERMAN);
    option->AppendRadioItem (myID_FRENCH, MENU_LANGUAGE_FRENCH);
    
    if (language==_T("ENGLISH")){
        option->Check(myID_ENGLISH, true);
    }
    if (language==_T("ITALIAN")){
        option->Check(myID_ITALIAN, true);
    }
    if (language==_T("SPANISH")){
        option->Check(myID_SPANISH, true);
    }
    if (language==_T("GERMAN")){
        option->Check(myID_GERMAN, true);
    }
    if (language==_T("FRENCH")){
        option->Check(myID_FRENCH, true);
    }
    
    
    // DOCUMENTS MENU
    wxMenu* docs = new wxMenu;
    if (pConfig->Read(_T("PDF1_NAME"), _T(""))!=_T("")){docs->Append(ID_Doc1, pConfig->Read(_T("PDF1_NAME"), _T("")));}
    if (pConfig->Read(_T("PDF2_NAME"), _T(""))!=_T("")){docs->Append(ID_Doc2, pConfig->Read(_T("PDF2_NAME"), _T("")));}
    if (pConfig->Read(_T("PDF3_NAME"), _T(""))!=_T("")){docs->Append(ID_Doc3, pConfig->Read(_T("PDF3_NAME"), _T("")));}
    if (pConfig->Read(_T("PDF4_NAME"), _T(""))!=_T("")){docs->Append(ID_Doc4, pConfig->Read(_T("PDF4_NAME"), _T("")));}
    if (pConfig->Read(_T("PDF5_NAME"), _T(""))!=_T("")){docs->Append(ID_Doc5, pConfig->Read(_T("PDF5_NAME"), _T("")));}
    if (pConfig->Read(_T("PDF6_NAME"), _T(""))!=_T("")){docs->Append(ID_Doc6, pConfig->Read(_T("PDF6_NAME"), _T("")));}
    if (pConfig->Read(_T("PDF7_NAME"), _T(""))!=_T("")){docs->Append(ID_Doc7, pConfig->Read(_T("PDF7_NAME"), _T("")));}
    if (pConfig->Read(_T("PDF8_NAME"), _T(""))!=_T("")){docs->Append(ID_Doc8, pConfig->Read(_T("PDF8_NAME"), _T("")));}

    // HELP MENU
    wxMenu* help = new wxMenu;
    wxMenuItem *about = new wxMenuItem(help, ID_About, MENU_HELP_ABOUT);
    about->SetBitmap(help_xpm);
    help->Append(about);

    // Main MenuBar    
    mb->Append(file, MENU_FILE);
    mb->Append(edit, MENU_EDIT);
    mb->Append(search, MENU_SEARCH);        
    mb->Append(project , MENU_PROJECT);            
    mb->Append(zcode, MENU_ZCODE);
    mb->Append(glulx, MENU_GLULX);    
    mb->Append(otree, MENU_OBJECTTREE);
    mb->Append(syntaxmenu, _T("Syntax"));    
    mb->Append(option, MENU_OPTIONS);
    mb->Append(docs, MENU_DOCUMENTS);        
    mb->Append(help, MENU_HELP);    
    
    return mb;
}
 
 
// ALBERO  |--------------------------------------------------------------------
wxTreeCtrl* MyFrame::CreateTreeCtrl()
{
    wxTreeCtrl* tree = new wxTreeCtrl(this, wxID_ANY,
                                      wxPoint(0,0), wxSize(160,250),
                                      wxTR_DEFAULT_STYLE | wxSIMPLE_BORDER);
                                      
    wxImageList* imglist = new wxImageList(16, 16, true, 2);
    imglist->Add(wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER, wxSize(16,16)));
    imglist->Add(wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16)));
    tree->AssignImageList(imglist);       
    return tree;
}

// TOOL BAR
wxToolBar* MyFrame::CreateToolBarCtrl()
{
    wxToolBar* tb2 = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                   wxTB_FLAT | wxTB_NODIVIDER);
    tb2->SetToolBitmapSize(wxSize(16,16));
   
   
    tb2->AddTool(ID_LoadFile, fileopen_xpm,MENU_FILE_OPENFILE);
    tb2->AddTool(ID_NewFile, new_xpm,MENU_FILE_NEWFILE);    
    tb2->AddTool(ID_Save_File, filesave_xpm, MENU_FILE_SAVEFILE);
//    tb2->AddTool(103, filesaveas_xpm, "Save File as...");
    tb2->AddTool(ID_Save_All, filesaveall_xpm, MENU_FILE_SAVEALL);
    tb2->AddSeparator(); 
    
    tb2->AddTool(ID_PreviousPage, back_xpm,MENU_FILE_PREVIOUSTAB);
    tb2->AddTool(ID_NextPage, forward_xpm,MENU_FILE_NEXTTAB);
    tb2->AddSeparator();

    tb2->AddTool(wxID_UNDO, undo_xpm,MENU_EDIT_UNDO);
    tb2->AddTool(wxID_REDO, redo_xpm,MENU_EDIT_REDO);
    tb2->AddSeparator(); 
    
    tb2->AddTool(wxID_COPY, copy_xpm,MENU_EDIT_COPY);
    tb2->AddTool(wxID_CUT, cut_xpm,MENU_EDIT_CUT);
    tb2->AddTool(wxID_PASTE, paste_xpm,MENU_EDIT_PASTE);
    tb2->AddTool(ID_FindBack, findback_xpm,MENU_SEARCH_FIND_BACKWARDS);   //PL
    tb2->AddTool(ID_Find, find_xpm,MENU_SEARCH_FIND);    //PL
//    tb2->AddTool(108, findrepl_xpm,"Find and Replace");
    tb2->AddTool(ID_FindObjectLocal, findobject_xpm,MENU_SEARCH_FIND_OBJECT);    //PL
    tb2->AddTool(ID_FindObjectGlobal, findobjectg_xpm,MENU_SEARCH_FIND_OBJECT_IN_FILE);    //PL    
    tb2->AddSeparator();
    
    tb2->AddTool(ID_Compile, compilezcode_xpm, MESSAGES_COMPILEZCODE);
    tb2->AddTool(ID_RunZcode, exezcode_xpm, MESSAGES_RUNZCODE);
    tb2->AddSeparator();
    
    tb2->AddTool(ID_CompileUlx, compileulx_xpm, MESSAGES_COMPILEULX);
    tb2->AddTool(ID_RunUlx, exeglulx_xpm, MESSAGES_RUNGLULX);        
    tb2->AddSeparator();
    
    tb2->AddTool(ID_About, help_xpm, MENU_HELP_ABOUT);
    tb2->AddTool(ID_Exit, quit_xpm, MENU_FILE_QUIT);
    tb2->Realize();
    
    return tb2;
}

  DECLARE_APP(MyApp);
  IMPLEMENT_APP(MyApp);
  
