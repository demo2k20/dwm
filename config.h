/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 0;        /* horiz inner gap between windows */
static const unsigned int gappiv    = 0;        /* vert inner gap between windows */
static const unsigned int gappoh    = 0;        /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 0;        /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 16;       /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const char *fonts[]          = { "Terminus:size=8" , "JoyPixels:size=8" };
static char normbgcolor[]           = "#222222";
static char urgentbgcolor[]         = "#ff0000";
static char normbordercolor[]       = "#444444";
static char urgentbordercolor[]     = "#ff0000";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
        /*               fg           bg             border   */
        [SchemeNorm] = { normfgcolor, normbgcolor,   normbordercolor   },
        [SchemeSel]  = { selfgcolor,  selbgcolor,    selbordercolor    },
        [SchemeUrg]  = { selfgcolor,  urgentbgcolor, urgentbordercolor },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* class       instance    title         tags mask     isfloating   monitor */
	{ NULL,        NULL,       "spterm",     0,            1,           -1 },
	{ NULL,        NULL,       "bcterm",     0,            1,           -1 },
	{ NULL,        NULL,       "floatterm",  0,            1,           -1 },
	{ NULL,        NULL,       "pulsemixer", 0,            1,           -1 },
	{ "Steam",     NULL,       NULL,         0,            1,           -1 },
	{ "zoom",      NULL,       NULL,         0,            1,           -1 },
	{ "discord",   NULL,       NULL,         1 << 8,       0,           -1 },
	{ "Element",   NULL,       NULL,         1 << 7,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50;  /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;     /* number of clients in master area */
static const int resizehints = 0;     /* 1 means respect size hints in tiled resizals */
static const int attachdirection = 4; /* 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */
static const int lockfullscreen = 1;  /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

#define STATUSBAR "dwmblocks"

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,             XK_i,      togglebar,      {0} }, // Toggle bar
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } }, // Focus downwards in the stack
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } }, // Focus upwards in the stack
	{ MODKEY|ShiftMask,             XK_j,      incnmaster,     {.i = +1 } }, // Increase number of masters
	{ MODKEY|ShiftMask,             XK_k,      incnmaster,     {.i = -1 } }, // Decrease number of masters
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} }, // Resize master window(s)
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} }, // Resize master window(s)
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} }, // Resize focused window
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} }, // Resize focused window
//	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY,                       XK_a,      zoom,           {0} }, // Bring focused window to master and back
    { MODKEY|ShiftMask,             XK_a,      focusmaster,    {0} }, // Focus master window and back
	{ MODKEY,                       XK_q,      killclient,     {0} }, // Close focused window
    { MODKEY,                       XK_s,      view,           {0} }, // Switch between two most recently used tags
	{ MODKEY,                       XK_e,      setlayout,      {0} }, // Switch between two most recently used layouts
	{ MODKEY|ShiftMask,		        XK_space,  cyclelayout,    {.i = +1 } }, // Cycle layouts
	{ MODKEY,                       XK_f,      togglefullscr,  {0} }, // Toggle fullscreen for a window
	{ MODKEY|ShiftMask,             XK_f,      togglefloating, {0} }, // Toggle floating for a window
	{ MODKEY|ShiftMask,             XK_s,      togglesticky,   {0} },
	{ MODKEY,                       XK_Tab,    focusmon,       {.i = +1 } }, // Focus next monitor
	{ MODKEY|ShiftMask,             XK_Tab,    tagmon,         {.i = +1 } }, // Send window to next monitor
	{ MODKEY|ControlMask,           XK_Tab,    tagswapmon,     {.i = +1 } }, // Swap currently selected workspaces with next monitor
	{ MODKEY,                       XK_F5,     xrdb,           {.v = NULL } }, // Reload xresources color settings
	{ MODKEY|ShiftMask,             XK_minus,  togglegaps,     {0} }, // Toggle all gaps
//	{ MODKEY|Mod4Mask,              XK_u,      incrgaps,       {.i = +1 } },
//	{ MODKEY|Mod4Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
//	{ MODKEY|Mod4Mask,              XK_i,      incrigaps,      {.i = +1 } },
//	{ MODKEY|Mod4Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
//	{ MODKEY|Mod4Mask,              XK_o,      incrogaps,      {.i = +1 } },
//	{ MODKEY|Mod4Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
//	{ MODKEY|Mod4Mask,              XK_6,      incrihgaps,     {.i = +1 } },
//	{ MODKEY|Mod4Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
//	{ MODKEY|Mod4Mask,              XK_7,      incrivgaps,     {.i = +1 } },
//	{ MODKEY|Mod4Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
//	{ MODKEY|Mod4Mask,              XK_8,      incrohgaps,     {.i = +1 } },
//	{ MODKEY|Mod4Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
//	{ MODKEY|Mod4Mask,              XK_9,      incrovgaps,     {.i = +1 } },
//	{ MODKEY|Mod4Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
//	{ MODKEY|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
//	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
//	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
//	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
//	{ MODKEY,                       XK_space,  setlayout,      {0} },
//	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
//	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_Escape, quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[5]} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigstatusbar,   {.i = 5} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

