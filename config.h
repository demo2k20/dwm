/* appearance */
static const unsigned int borderpx  = 2;         /* border pixel of windows */
static const unsigned int snap      = 16;        /* snap pixel */
static const unsigned int gappih    = 8;         /* horiz inner gap between windows */
static const unsigned int gappiv    = 8;         /* vert inner gap between windows */
static const unsigned int gappoh    = 8;        /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 8;        /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;         /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;         /* 0 means no bar */
static const int topbar             = 1;         /* 0 means bottom bar */
static const int user_bh            = 20;        /* 0 means default bar height */
static const char *fonts[]          = { "DejaVu Sans Mono:size=9", "Font Awesome 5 Free Regular:size=8", "Font Awesome 5 Free Solid:size=8", "Font Awesome 5 Brands Regular:size=8" };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg              border   */
       [SchemeNorm] = { normfgcolor, normbgcolor,    normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,     selbordercolor  },
       [SchemeUrg]  = { selfgcolor,  selbordercolor, selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* class       instance    title        tags mask     isfloating   monitor */
	{ "Alacritty", NULL,       "spterm",    0,            1,           -1 },
	{ "Alacritty", NULL,       "floatterm", 0,            1,           -1 },
	{ "discord",   NULL,       NULL,        1 << 8,       0,           -1 },
	{ "Steam",     NULL,       NULL,        0,            1,           -1 },
	{ "guvcview",  NULL,       NULL,        0,            1,           -1 },
	{ "Guvcview",  NULL,       NULL,        0,            1,           -1 },
	{ "Guvcview",  NULL,       NULL,        1 << 8,       1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle }, /* fullscreen with bar */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },    /* NULL to end the function */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,             XK_i,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_a,      zoom,           {0} },
    { MODKEY,                       XK_s,      view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_e,      cyclelayout,    {.i = +1 } },
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_Tab,    focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Tab,    tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_minus,  togglegaps,     {0} },
	{ MODKEY,                       XK_F5,     xrdb,           {.v = NULL } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY,                       XK_Escape,      quit,      {1} },
	{ MODKEY|ShiftMask,             XK_Escape,      quit,      {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
