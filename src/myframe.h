#ifndef _MYFRAME_H_
#define _MYFRAME_H_

  #include "wx/aui/aui.h"
  #include "wx/treectrl.h"

  // COSTANTI
  #define SEP " - "
  #define VERSIONE "1.01.01"
  #define BUILD " (build 201307150915) "
  #define NOMEAPPLICAZIONE "WIDE"  
  #define DESCRIZIONE "Wx Inform Development Environment"    
  #define CONFIG_FILE "wide.ini"  

  // Icone
  #include "images/wide.xpm"

class Edit;

class MyFrame : public wxFrame {
 enum
    {
        ID_LoadFile = wxID_HIGHEST+1000,
        ID_NewFile,        
        ID_Exit,
        ID_About,
        ID_Comment,
        ID_Uncomment,        
        ID_License,
        ID_Save_File,
        ID_Save_All,        //PL
        ID_NB_Close,
        ID_NextPage,
        ID_PreviousPage,
        ID_ShowObject,
        ID_ShowProject,
        ID_ShowGlobal,
        ID_ShowConstant,
        ID_ShowFunction,
        ID_ShowClass,
        ID_ShowInclude,
        ID_ShowVerb, 
        ID_ExpandAllAlways,
        ID_GotoLine,
        ID_Find,
        ID_FindBack,     //PL
        ID_FindReplace,
        ID_FindObjectLocal,
        ID_FindObjectGlobal,
        ID_NextMarker,
        ID_ToggleMarker,
        ID_ResetMarkers,
        ID_RefreshTree,
        ID_Compile,
        ID_CompileUlx,                
        ID_RunZcode,
        ID_RunUlx,  
        ID_RunBlb,      // AS
        ID_ZcodeVersion5,
        ID_ZcodeVersion8,        
        ID_Doc1,            
        ID_Doc2,            
        ID_Doc3,                            
        ID_Doc4,    
        ID_Doc5,            
        ID_Doc6,
        ID_Doc7,
        ID_Doc8,        
        ID_Tip,              
        ID_OpenProject,           
        ID_SaveProject,         
        ID_CloseProject,
        ID_CreateBlb,    
        ID_MakeAllBlb,
        ID_CreateRes,    
        ID_CreateZBlb,
        ID_RunZBlb,    
        
        ID_UseDirectives,
        ID_UseStatements,
        ID_UseOtherkeywords,
        ID_UseInformate,
        ID_UseCustom1,
        ID_UseCustom2,
        ID_UseCustom3,       
        
        ID_Fine = ID_Exit+1000
    };    
    

   
 public:
   MyFrame(wxWindow* parent, wxPoint p, int width, int height) : wxFrame(parent, -1, 
        _(NOMEAPPLICAZIONE SEP VERSIONE BUILD),
                      p, wxSize(width,height),
                      wxDEFAULT_FRAME_STYLE)                              
   {
     untitled=0;
     mainFile=_T("");
     LoadConfiguration();
     zcodeversion=_T(".z5");
     zcodeswitch=_T("-v5");
     
     // notify wxAUI which frame to use
     m_mgr.SetManagedWindow(this);
    
     SetIcon(wxIcon(wide_xpm));
    
     // Aggiungo la menubar
     SetMenuBar(CreateMenuBar());
     CreateStatusBar();
     GetStatusBar()->SetStatusText(MESSAGES_READY);
 
/*     console = new wxTextCtrl(this, -1, _(""),
                      wxDefaultPosition, wxSize(100,100),
                      wxNO_BORDER | wxTE_MULTILINE);
     console->SetEditable(false);        */
     
     console = new wxStyledTextCtrl(this, -1,
                     wxDefaultPosition, wxSize(100,100),
                     wxNO_BORDER | wxTE_MULTILINE, _T("console"));
     console->SetReadOnly(true);   
         
     // add the panes to the manager
     tree = CreateTreeCtrl();
     m_mgr.AddPane(tree, wxLEFT, MENU_OBJECTTREE);
     m_mgr.AddPane(auinotebook=CreateNotebook(), wxCENTER);
     m_mgr.AddPane(console, wxBOTTOM, MESSAGES_OUTPUTWINDOW);
                        
    // TOOL BAR
    toolbar = CreateToolBarCtrl();
    m_mgr.AddPane(toolbar, wxAuiPaneInfo().
          Name(wxT("tb2")).Caption(wxT("Toolbar 2")).
          ToolbarPane().Top().Row(1).
          LeftDockable(false).RightDockable(false));                        
                              
     // tell the manager to "commit" all the changes just made
     m_mgr.Update();

   }
 
   ~MyFrame()
   {
     SaveConfiguration();
     // deinitialize the frame manager
     m_mgr.UnInit();
   }
 
 public:
    void RefreshTree();

 // Funzioni
 private:
    
    // FILE MENU
    void OnNewFile(wxCommandEvent& evt);    
    void OnLoadFile(wxCommandEvent& evt);
    void LoadFile(wxString path, wxString name);    
    bool checkOpenFile(wxString path);          //AS: check if file is opened
    void setNewStc(Edit* stc);                  //PL: New stc settings
    void OnSaveFile(wxCommandEvent &event); 
    void OnSaveAll(wxCommandEvent &event);       //PL
    void OnLanguage(wxCommandEvent &event);      
    void OnExit(wxCommandEvent& evt);
    void OnNextPage(wxCommandEvent& event);
    void OnPreviousPage(wxCommandEvent& event);

    // MENU EDIT
    void OnComment(wxCommandEvent& event);
    void OnUncomment(wxCommandEvent& event);    
                
    // MENU ABOUT
    void OnAbout(wxCommandEvent& evt);
    void OnLicense(wxCommandEvent& evt);
    void OnOptions(wxCommandEvent &event);   
    void LoadConfiguration();   
    void SaveConfiguration();
    void OnSyntax(wxCommandEvent &event);   
   
    // Search menu
    void GotoLine(wxCommandEvent &event);   
    void Search(wxCommandEvent &event);
    void SearchBack(wxCommandEvent &event);   //PL
    void SearchObjectLocal(wxCommandEvent &event) { SearchObject(false); };
    void SearchObjectGlobal(wxCommandEvent &event) { SearchObject(true); };
    void SearchObject(bool);   
    size_t SearchRegularExpression(wxString text, wxString pattern);    
    void FindReplace(wxCommandEvent &event);   
    void NextMarker(wxCommandEvent &event);
    void ToggleMarker(wxCommandEvent &event);
    void ResetMarkers(wxCommandEvent &event);
   
    // Tree
    void OnUpdateTree();   
    void OnRefreshTree(wxCommandEvent &event);
    void OnUpdateTreeRegularExpression(wxString text, wxTreeItemId root, wxString nome,  wxString pattern, bool keepquote);
    
    // Metodi lanciati su stc    
    void OnEdit (wxCommandEvent &event);
    void OnSingleEdit (wxCommandEvent &event);    

    // Project Menu
    void OnOpenProject(wxCommandEvent &event);
    void OnSaveProject(wxCommandEvent &event);
    void OnCloseProject(wxCommandEvent &event);
    
    // ZCode menu
    void OnCompile (wxCommandEvent &event);
    void OnRunZcode(wxCommandEvent &event);
    void OnZcodeVersion(wxCommandEvent &event);
    void OnCreateZBlb(wxCommandEvent &event);
    void OnRunZBlb(wxCommandEvent &event);
    // Glulx Menu
    void OnCompileUlx(wxCommandEvent &event);
    void OnRunUlx(wxCommandEvent &event);        
    void OnRunBlb(wxCommandEvent &event);        
    void OnCreateBlb(wxCommandEvent &event);    
    void OnCreateRes(wxCommandEvent &event);        
    void OnMakeAllBlb(wxCommandEvent &event);
        
    // Metodi generici
    void OnClose (wxCloseEvent &event);
    void OnNotebookPageClose(wxAuiNotebookEvent& evt);
    void OnNotebookPageChanged(wxAuiNotebookEvent& evt);
    void OnSelChanged(wxTreeEvent &event);
    
    // Metodi della console
    void OnConsoleClicked(wxStyledTextEvent &event);
    void OnOutput(wxString string);
    void OnClear();

    // HELP MENU
    void OnViewDoc(wxCommandEvent &event);
   
 private:
     wxAuiManager m_mgr;

     wxAuiNotebook* auinotebook;
//     wxTextCtrl* console;
     wxStyledTextCtrl* console;
     wxToolBar* toolbar;
     wxTreeCtrl* tree;
     
     wxMenu* zcodemenu;
     
     wxString lastSearch;
     wxString lastObSearch;

     
     // Impostazioni Object tree
     bool expandAllNodes;
     bool showObjects;    
     bool showProject; 
     bool showGlobals;     
     bool showConstants;     
     bool showFunctions;     
     bool showClasses;     
     bool showIncludes;     
     bool showVerbs;        
     
     // syntax
     bool useOtherkeywords;
     bool useDirectives;
     bool useStatements;
     bool useInformate;
     bool useCustom1;
     bool useCustom2;
     bool useCustom3;

     int untitled;
     int autoCompleteNumber;    // Number of char typed before window autocomplete     
     int tabSize;
     bool autoCompleteSwitch;
     bool showLineNumber;
     bool wrapMode;
     bool hotkeysSwitch;

     // TRANSLATION STUFF
     wxString language;
     wxString MENU_FILE;
     wxString MENU_FILE_NEWFILE; 
     wxString MENU_FILE_OPENFILE;   
     wxString MENU_FILE_SAVEFILE;   
     wxString MENU_FILE_SAVEALL;
     wxString MENU_FILE_NEXTTAB;    
     wxString MENU_FILE_PREVIOUSTAB;
     wxString MENU_FILE_QUIT;    
     
     wxString MENU_EDIT;             
     wxString MENU_EDIT_COPY;            
     wxString MENU_EDIT_CUT;
     wxString MENU_EDIT_PASTE;
     wxString MENU_EDIT_UNDO;
     wxString MENU_EDIT_REDO;
     wxString MENU_EDIT_INDENT;
     wxString MENU_EDIT_UNINDENT;
     wxString MENU_EDIT_COMMENT;
     wxString MENU_EDIT_UNCOMMENT;

     wxString MENU_SEARCH;             
     wxString MENU_SEARCH_FIND;                  
     wxString MENU_SEARCH_FIND_BACKWARDS;                       
     wxString MENU_SEARCH_FIND_REPLACE;                       
     wxString MENU_SEARCH_FIND_OBJECT;
     wxString MENU_SEARCH_FIND_OBJECT_IN_FILE;                                         
     wxString MENU_SEARCH_FIND_GOTO_LINE;                       
     wxString MENU_SEARCH_FIND_NEXT_MARKER;                       
     wxString MENU_SEARCH_FIND_TOGGLE_MARKER;                            
     wxString MENU_SEARCH_FIND_RESET_MARKERS;                            

     wxString MENU_PROJECT;
     wxString MENU_PROJECT_OPEN;
     wxString MENU_PROJECT_SAVE;
     wxString MENU_PROJECT_CLOSE;          
     
     wxString MENU_LANGUAGE;
     wxString MENU_LANGUAGE_ENGLISH;
     wxString MENU_LANGUAGE_ITALIAN;
     wxString MENU_LANGUAGE_SPANISH;
     wxString MENU_LANGUAGE_GERMAN;
     wxString MENU_LANGUAGE_FRENCH;
     
     
     wxString MENU_ZCODE;
     wxString MENU_ZCODE_COMPILEZCODE;
     wxString MENU_ZCODE_RUNZCODE;
     wxString MENU_ZCODE_ZCODE5;
     wxString MENU_ZCODE_ZCODE8;
     wxString MENU_ZCODE_COMPILEZCODE_BLORB;
     wxString MENU_ZCODE_RUNZCODE_BLORB;
     
     wxString MENU_GLULX;
     wxString MENU_GLULX_COMPILEULX;
     wxString MENU_GLULX_RUNULX;
     wxString MENU_GLULX_CREATERESOURCES;
     wxString MENU_GLULX_CREATEBLORBFILE;
     wxString MENU_GLULX_BUILDALLBLORBFILE;
     wxString MENU_GLULX_RUNBLORB;
     
     wxString MENU_OBJECTTREE;
     wxString MENU_OBJECTTREE_REFRESHTREE;
     wxString MENU_OBJECTTREE_EXPAND_NODES;
     wxString MENU_OBJECTTREE_SHOW_OBJECTS;
     wxString MENU_OBJECTTREE_SHOW_PROJECT_DEFS;
     wxString MENU_OBJECTTREE_SHOW_GLOBALS;
     wxString MENU_OBJECTTREE_SHOW_CONSTANTS;
     wxString MENU_OBJECTTREE_SHOW_FUNCTIONS;                              
     wxString MENU_OBJECTTREE_SHOW_CLASSES;                              
     wxString MENU_OBJECTTREE_SHOW_INCLUDES;                              
     wxString MENU_OBJECTTREE_SHOW_VERBS;                              
     
     wxString MENU_OPTIONS;
     wxString MENU_OPTIONS_SHOWLINENUMBER;
     wxString MENU_OPTIONS_WRAPMODE;
     wxString MENU_OPTIONS_AUTOCOMPLETE;
     wxString MENU_OPTIONS_HOTKEYSREPLACING;
     
     wxString MENU_DOCUMENTS;
     
     wxString MENU_HELP;
     wxString MENU_HELP_LICENSE;
     wxString MENU_HELP_ABOUT;     
     
     
     // GENERIC MESSAGES AND ERRORS
    wxString MESSAGES_READY;
    wxString MESSAGES_OUTPUTWINDOW;
    wxString MESSAGES_SEARCH_ENTERSTRING;
    wxString MESSAGES_SEARCH_SEARCHSTRING;
    wxString MESSAGES_MESSAGE;
    wxString MESSAGES_SEARCHOBJECT;
    wxString MESSAGES_SEARCHOBJECTFILES;
    wxString MESSAGES_SEARCHOBJECT_ENTEROBJECT;
    wxString MESSAGES_LINENUMBER;
    wxString MESSAGES_GOTOLINENUMBER;
    wxString MESSAGES_FILESAVED;
    wxString MESSAGES_TEXTNOTSAVED;
    wxString MESSAGES_FILEOPENED;
    wxString MESSAGES_WARNING;
    wxString MESSAGES_REMOVEMAINFILE;
    wxString MESSAGES_MAINFILEREMOVED;
    wxString MESSAGES_NOPROJECTOPENED;
    wxString MESSAGES_COMPILEULX;
    wxString MESSAGES_COMPILEZCODE;
    wxString MESSAGES_CREATINGBLORB;
    wxString MESSAGES_CREATINGRESOURCEZBLORB;
    wxString MESSAGES_CREATINGZBLORB;
    wxString MESSAGES_CREATINGRESOURCEBLORB;
    wxString MESSAGES_RUNZBLORB;
    wxString MESSAGES_RUNZCODE;
    wxString MESSAGES_RUNBLORB;
    wxString MESSAGES_RUNGLULX;
    wxString MESSAGES_SAVINGALL;
    wxString MESSAGES_USINGMAINFILE;
    wxString ERRORS_ERROR;
    wxString ERRORS_RE;
    wxString ERRORS_FILEEXISTS;
    wxString ERRORS_FILEACCESSERROR;
    wxString ERRORS_NOTIMPLEMENTED;
    wxString ERRORS_NOMARKERS;
    wxString ERRORS_SAVEERROR;
    wxString ERRORS_CLOSEABORT;
     
     
     // INFORM STUFF
     wxString informCompiler;
     wxString libraryPath;     
     wxString zcodeversion;          
     wxString zcodeswitch;    
     
     wxString bres;         
     wxString blc;  
     wxString bext;            
     
     wxString mainFile;      
     wxArrayString projkeywords;
     wxArrayString projclasses;
     
     bool inform_error;            

     // Funzioni di inzializzazione componenti
     wxTreeCtrl* CreateTreeCtrl();      // Definizione creazione albero
     wxMenuBar* CreateMenuBar();
     wxAuiNotebook* CreateNotebook();
     wxToolBar* CreateToolBarCtrl();
     
     bool debugflag;
        
    
     DECLARE_EVENT_TABLE()
 };
#endif
