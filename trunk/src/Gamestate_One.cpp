/* This file is part of the Zenipex Library.
* Copyleft (C) 2010 Mitchell Keith Bloch a.k.a. bazald
*
* This source file is simply under the public domain.
*/

#include <zenilib.h>
#include "MyTitleState.h"


using namespace std;
using namespace Zeni;

static vector<string> g_args;

namespace Zeni {

  /** Do NOT add any other code to the Zeni namespace unless you intend to modify zenilib.
   *  Modifying the functions below should be the extent of your modifications to zenilib.
   *
   *  If you merely intend to use zenilib, your code should be in a namespace unique
   *  to your game, or in the global namespace.
   *
   *  Access classes and functions in zenilib by prefixing them with Zeni:: in your
   *  header files.  It is acceptable to use 'using' directives in source files only.
   *
   *  All your source files should include <zenilib.h> in the first line, or certain
   *  build targets will give you errors involving precompiled headers.
   */

  Gamestate_One::Gamestate_One(const vector<string> * const args) {
    if(args)
      g_args = *args;

    Video::set_title("stranDead");

    get_Video();
    get_Textures();
    get_Fonts();
    get_Sounds();

    get_Video().set_2d(make_pair(Point2f(0,0), Point2f(1088,612)), true);
  }

  Gamestate_One::~Gamestate_One() {
  }

  void Gamestate_One::perform_logic() {
    // TODO: Parse commandline arguments and bootstrap into titlestate or other gamestate;
    Game &gr = get_Game();
    gr.pop_state();
    gr.push_state(new MyTitleState());
  }

}
