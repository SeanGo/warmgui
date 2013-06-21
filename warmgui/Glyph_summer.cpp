#include "StdAfx.h"
#include "warmgui_summer.h"

namespace WARMGUI {
IGlyph_summer::IGlyph_summer(void)
    : _own_artist(GLYPH_OWN_ARTIST_TYPE_OUTSIDE)
    , _artist(0)
    //, _my_artist(0)
    , _own_world(GLYPH_OWN_WORLD_TYPE_OUTSIDE)
    , _world(0)
    , _my_world(0)
    , _glyph_tree(0)
    , _tree_iter(0)
    , _config(0)
{
    memset(&_rect, 0 , sizeof(RECT));
    _abs_rect = _rect;
    *_name = *_conf_str = '\0';
}

IGlyph_summer::IGlyph_summer(const char* name)
    : _own_artist(GLYPH_OWN_ARTIST_TYPE_OUTSIDE)
    , _artist(0)
    //, _my_artist(0)
    , _own_world(GLYPH_OWN_WORLD_TYPE_OUTSIDE)
    , _world(0)
    , _my_world(0)
    , _glyph_tree(0)
    , _tree_iter(0)
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
    /*
    _artist->GetTransform(&_back_trans);
    MATRIX_2D I = D2D1::IdentityMatrix();
    I._31 = (float)_abs_rect.left, I._32 = (float)_abs_rect.top;
    _artist->SetTransform(&I);
    */
    return S_OK;
}

inline HRESULT IGlyph_summer::pop_layer()
{
#ifdef _DEBUG
    TCHAR name[MAX_PATH];
    CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, _name);
    MYTRACE(L"Graph::POP layer %s\n", name);
#endif //_DEBUG
    //_artist->SetTransform(&_back_trans);
    return S_OK;
}

inline HRESULT pop_layer();
HRESULT IGlyph_summer::draw_graph()
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
                    hr = (*it)->draw_graph();
                    if (FAILED(hr))
                        break;
                }
            }

            if (SUCCEEDED(hr) && _tree_iter != _glyph_tree->begin())
                hr = pop_layer();
        } else {
            hr = draw();
        }

        return hr;
    }

    return ERROR_GLYPH_TREE;
}

/*
HRESULT IGlyph_summer::draw_graph()
{
    HRESULT hr = S_OK;
    if (_artist && _glyph_tree && (*_tree_iter)) {
        if (
            _tree_iter != _glyph_tree->begin()   //is not the head of the tree
            && _tree_iter.node->prev_sibling     //is not first node in the branch or subtree
            )
        {
            hr = draw_myself();
        } else {//draw all brach in the same deepth
            hr = draw_myself();

            if (SUCCEEDED(hr)) {//draw all next sibling
                for (int i = 1; i < _glyph_tree->number_of_siblings(_tree_iter); i++) {
                    GlyphTreeIter_summer it = _glyph_tree->sibling(_tree_iter, (unsigned int)i);
                    hr = (*it)->draw_graph();
                    if (FAILED(hr))
                        break;
                }
            }
        }
        return hr;
    }
    return ERROR_GLYPH_TREE;
}
*/

inline void IGlyph_summer::append_glyph(IGlyph_summer* glyph)
{
    GlyphTreeIter_summer glyph_iter = _glyph_tree->append_child(this->_tree_iter, glyph);
    glyph->inherit(_atelier, _glyph_tree, _canvas, glyph_iter, _artist, _config);
}



} //namespace WARMGUI

/*
eArtist* back_artist = _artist;
        D2D1_RECT_F dest = {0, 0, fRectWidth(_rect), fRectHeight(_rect)};
        D2D1_RECT_F src  = {0, 0, fRectWidth(_rect), fRectHeight(_rect)};

        MATRIX_2D backmatrix, m;
        _artist->GetTransform(&backmatrix);
        m = backmatrix;
        m._31 = (float)_rect.left, m._32 = (float)_rect.top;
        _artist->SetTransform(&m);
*/