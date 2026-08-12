#ifndef SONG_MAN_H
#define SONG_MAN_H

#include "../buzzer.h"





const sound man_voice1 []=
{
    {2},

    {F5,2},
    {C5,2},
    {Bb5,2},
    {Bb5,4},

    {Bb5,1},
    {C5,1},
    {D6,1},
    {Bb5,1},
    {Bb5,1},
    {C5,1},
    {F5,2},

    {C5,2},
    {Bb5,2},
    {Bb5,4},

    {Bb5,1},
    {C5,1},
    {D6,1},
    {Bb5,1},
    {Bb5,1},
    {C5,1},
    {F5,2},

    {C5,2},
    {Bb5,2},
    {Bb5,4},

    {Bb5,1},
    {C5,1},
    {D6,1},
    {Bb5,1},
    {Bb5,1},
    {C5,1},
    {F5,2},

    {C5,2},
    {Bb5,2},
    {Bb5,4},


};
const sound man_voice2 [] =
{
    {2},

    {D5,2},
    {2},
    {2},
    {D5,4},

    {1},
    {1},
    {1},
    {C5,1},
    {C5,1},
    {C5,1},
    {Bb5,2},

    {C5,2},
    {Bb5,2},
    {F5,4},

    {1},
    {1},
    {1},
    {C5,1},
    {C5,1},
    {C5,1},
    {Bb5,2},

    {C5,2},
    {Bb5,2},
    {F5,4},

    {1},
    {1},
    {1},
    {C5,1},
    {C5,1},
    {C5,1},
    {Bb5,2},

    {C5,2},
    {Bb5,2},
    {F5,4},


};
const sound man_voice3 []=
{
    {2},

    {G5,2},
    {2},
    {2},
    {F5,4},

    {1},
    {1},
    {1},
    {C5,1},
    {Bb5,1},
    {1},
    {G5,2},

    {2},
    {2},
    {D5,4},

    {1},
    {1},
    {1},
    {C5,1},
    {Bb5,1},
    {1},
    {G5,2},

    {2},
    {2},
    {D5,4},
    {1},
    {1},
    {1},
    {C5,1},
    {Bb5,1},
    {1},
    {G5,2},

    {2},
    {2},
    {D5,4},
};
const sound man_voice4 []=
{
    {2},

    {Bb5,2},
    {2},
    {2},
    {C5,4},

    {1},
    {1},
    {1},
    {1},
    {1},
    {1},
    {Eb5,2},
    
    {2},
    {2},
    {C5,4},

    {1},
    {1},
    {1},
    {1},
    {1},
    {1},
    {Eb5,2},
    
    {2},
    {2},
    {C5,4},

    {1},
    {1},
    {1},
    {1},
    {1},
    {1},
    {Eb5,2},
    
    {2},
    {2},
    {C5,4},

};










const song Man{
    man_voice1,
    man_voice2,
    man_voice3,
    man_voice4,
    nullptr,
    sizeof(man_voice1)/sizeof(sound),
    sizeof(man_voice2)/sizeof(sound),
    sizeof(man_voice3)/sizeof(sound),
    sizeof(man_voice4)/sizeof(sound),
    0,
    100};
#endif















