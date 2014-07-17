#ifndef _MYTREEITEMDATA_H_
#define _MYTREEITEMDATA_H_

class MyTreeItemData : public wxTreeItemData
{
public:
    MyTreeItemData(const wxString& desc) : m_desc(desc) { }
    MyTreeItemData(int pos) : posizione(pos) { }
    MyTreeItemData(int pos, int lun) : posizione(pos),lunghezza(lun)  { }

    void ShowInfo(wxTreeCtrl *tree);
    const wxChar *GetDesc() const { return m_desc.c_str(); }
    int GetPosition() { return posizione; }    
    int GetLength() { return lunghezza; }    
    
private:
    wxString m_desc;
    int posizione;
    int lunghezza;
};
#endif
