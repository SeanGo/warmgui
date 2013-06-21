#include "stdafx.h"
#include "warmgui_summer.h"

#include "test-unit.h"

int test_generate_glyph_tree(HWND hwnd)
{
    WARMGUI::IAtelier_summer* summer = new WARMGUI::IAtelier_summer("atelier");
    summer->init(hwnd);

    for (int j = 1; j <= 5; j++) {
        char canvas_name[MAX_PATH];
        _snprintf_s(canvas_name, MAX_PATH, _TRUNCATE, "canvas %d", j);
        WARMGUI::ICanvas_summer* canvas = new WARMGUI::ICanvas_summer(canvas_name);
        summer->append_canvas(canvas);
    }

    for (int j = 1; j <= 5; j++) {
        char canvas_name[MAX_PATH];
        _snprintf_s(canvas_name, MAX_PATH, _TRUNCATE, "canvas %d", j);
        WARMGUI::ICanvas_summer* canvas = (WARMGUI::ICanvas_summer*)summer->find_glyph(canvas_name);

        if (canvas) {
            char glyph_name[MAX_PATH];
            for (int i = 0; i < 5; i++) {
                _snprintf_s(glyph_name, MAX_PATH, _TRUNCATE, "%s-glyph %d", 
                    canvas->get_name(),
                    i);

                WARMGUI::IGlyph_summer* glyph = new WARMGUI::IGlyph_summer(glyph_name);
                canvas->append_glyph(glyph);
            }
        }
    }


    for (int j = 1; j <= 5; j++) {
        char canvas_name[MAX_PATH];
        _snprintf_s(canvas_name, MAX_PATH, _TRUNCATE, "canvas %d", j);
        WARMGUI::ICanvas_summer* canvas = (WARMGUI::ICanvas_summer*)summer->find_glyph(canvas_name);

        if (canvas) {
            char glyph_name[MAX_PATH];
            for (int i = 0; i < 5; i++) {
                _snprintf_s(glyph_name, MAX_PATH, _TRUNCATE, "%s-glyph %d", 
                    canvas->get_name(),
                    i);

                WARMGUI::IGlyph_summer* old_glyph = summer->find_glyph(glyph_name);

                if (old_glyph) {
                    for (int k = 0; k < 3; k++) {
                        _snprintf_s(glyph_name, MAX_PATH, _TRUNCATE, "%s-subglyph %d", 
                            old_glyph->get_name(),
                            k);
                        WARMGUI::IGlyph_summer* glyph = new WARMGUI::IGlyph_summer(glyph_name);
                        if (!k) glyph->set_glyph_type(WARMGUI::IGlyph_summer::GLYPH_TYPE_BKG);
                        old_glyph->append_glyph(glyph);
                    }
                }
            }
        }
    }

    summer->change(GLYPH_CHANGED_ATELIER_RESIZE);
    summer->Draw();
    MYTRACE(L"\n\n");
    
    //do not changed anything
    summer->Draw();
    MYTRACE(L"\n\n");

    //change the bkg
    WARMGUI::IGlyph_summer* glyph = summer->find_glyph("canvas 2-glyph 3-subglyph 0");
    if (glyph)
        glyph->change(GLYPH_CHANGED_GLYPH_BKG);
    summer->Draw();
    MYTRACE(L"\n\n");

    //change the graph
    glyph = summer->find_glyph("canvas 2-glyph 2-subglyph 2");
    if (glyph)
        glyph->change(GLYPH_CHANGED_CHANGED);
    summer->Draw();
    MYTRACE(L"\n\n");

    //do not changed anything
    summer->Draw();
    MYTRACE(L"\n\n");

    SafeDelete(summer);
    return (0);
}