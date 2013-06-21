#include "StdAfx.h"
#include "warmgui_summer.h"

namespace WARMGUI {
IGlyph_summer::IGlyph_summer(void)
    : _own_artist(GLYPH_OWN_ARTIST_TYPE_OUTSIDE)
    , _artist(0)
    //, _my_artist(0)
    , _glyph_tree(0)
    , _tree_iter(0)
    , _config(0)
    , _changed(0)
    , _atelier(0)
    , _canvas(0)
    , _mytype(GLYPH_TYPE_GLYPH)
{
    memset(&_rect, 0 , sizeof(RECT));
    _abs_rect = _rect;
    *_name = *_conf_str = '\0';
}

IGlyph_summer::IGlyph_summer(const char* name)
    : _own_artist(GLYPH_OWN_ARTIST_TYPE_OUTSIDE)
    , _artist(0)
    //, _my_artist(0)
    , _glyph_tree(0)
    , _tree_iter(0)
    , _changed(0)
    , _atelier(0)
    , _canvas(0)
    , _mytype(GLYPH_TYPE_GLYPH)
{
    memset(&_rect, 0 , sizeof(RECT));
    _abs_rect = _rect;
    strcpy_s(_name, MAX_PATH, name);
    *_conf_str = '\0';
}

IGlyph_summer::~IGlyph_summer(void)
{
    if (_own_artist == GLYPH_OWN_ARTIST_TYPE_MYSELF)
        SafeDelete(_artist);
}


bool IGlyph_summer::is_me(const char* name)
{
    return (!strcmp(name, _name));
}

HRESULT IGlyph_summer::draw()
{
#ifdef _DEBUG
    TCHAR name[MAX_PATH];
    CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, _name);
    MYTRACE(L"Graph::draw %s\n", name);
#endif //_DEBUG

    return S_OK;
}

inline void IGlyph_summer::set_config(CWarmguiConfig* config, const char *conf_str)
{
    _config = config; strcpy_s(_conf_str, MAX_PATH, conf_str);
}

inline HRESULT IGlyph_summer::push_layer()
{
#ifdef _DEBUG
    TCHAR name[MAX_PATH];
    CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, _name);
    MYTRACE(L"Graph::PUSH layer %s %d, %d, %d, %d\n", name, _abs_rect.left, _abs_rect.top, _abs_rect.right, _abs_rect.bottom);
#endif //_DEBUG
    _artist->GetTransform(&_back_trans);
    MATRIX_2D I = D2D1::IdentityMatrix();
    I._31 = (float)_abs_rect.left, I._32 = (float)_abs_rect.top;
    _artist->SetTransform(&I);
    _artist->PushLayer(_rect);
    return S_OK;
}

inline HRESULT IGlyph_summer::pop_layer()
{
#ifdef _DEBUG
    TCHAR name[MAX_PATH];
    CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, _name);
    MYTRACE(L"Graph::POP layer %s\n", name);
#endif //_DEBUG
    _artist->PopLayer();
    _artist->SetTransform(&_back_trans);
    return S_OK;
}


HRESULT IGlyph_summer::draw_graph(GLYPH_TYPE glyph_type /*=IGlyph_summer::GLYPH_TYPE_GLYPH*/)
{
    HRESULT hr = S_OK;

    if (_artist && _glyph_tree && (*_tree_iter)) {
        int num_of_children = _glyph_tree->number_of_children(_tree_iter);
        if (num_of_children) {
            if (SUCCEEDED(hr) && _tree_iter != _glyph_tree->begin())
                hr = push_layer();

            if (SUCCEEDED(hr)) {
                for (int i = 0; i < num_of_children; i++) {
                    //draw all 
                    GlyphTreeIter_summer it = _glyph_tree->child(_tree_iter, (unsigned int)i);
                    hr = (*it)->draw_graph(glyph_type);
                    if (FAILED(hr))
                        break;
                }
            }

            if (SUCCEEDED(hr) && _tree_iter != _glyph_tree->begin())
                hr = pop_layer();
        } else {
            //set transform
            if (this->_mytype == glyph_type) {
                _artist->GetTransform(&_back_trans);
                MATRIX_2D I = D2D1::IdentityMatrix();
                I._31 = (float)_abs_rect.left, I._32 = (float)_abs_rect.top;
                _artist->SetTransform(&I);

                hr = draw();

                //get back for transform
                _artist->SetTransform(&_back_trans);
                set_change(GLYPH_CHANGED_NONE);
            }
        }

        return hr;
    }

    return ERROR_GLYPH_TREE;
}


inline void IGlyph_summer::append_glyph(IGlyph_summer* glyph)
{
    GlyphTreeIter_summer glyph_iter = _glyph_tree->append_child(this->_tree_iter, glyph);
    glyph->inherit(_atelier, _glyph_tree, _canvas, glyph_iter, _artist, _config);
}

inline void IGlyph_summer::set_rect(RECT& rect)
{
    _abs_rect = rect;
    _rect.left = _rect.top = 0, _rect.right = RectWidth(_rect), _rect.bottom = RectHeight(_rect);
    set_children_rect();
}

inline void IGlyph_summer::set_children_rect()
{
    for (int i = 0; i < _glyph_tree->number_of_children(_tree_iter); i++) {
        //draw all 
        GlyphTreeIter_summer it = _glyph_tree->child(_tree_iter, (unsigned int)i);
        (*it)->set_rect(_abs_rect);
    }
}

inline void IGlyph_summer::change(GLYPH_CHANGED_TYPE     changed)
{
    CriticalLock::Scoped scope(_lockChange);
    _changed |= changed ;
    
    if (_atelier) _atelier->change(changed);
    if (_canvas)   _canvas->change(changed);
}

} //namespace WARMGUI
