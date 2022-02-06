/* See LICENSE file for copyright and license details. */

/* Import XF86 library for binding functions keys */
#include <X11/XF86keysym.h>

/* Variables */
#define TERMINAL "st"

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 16;       /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const char *fonts[]          = { "Terminus:size=8" , "Font Awesome 5 Free Solid:size=8" , "JoyPixels:size=8" };
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

/* Scratchpads */
typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-t", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {TERMINAL, "-t", "spfm", "-g", "144x41", "-e", "ranger", NULL };
const char *spcmd3[] = {TERMINAL, "-t", "spcalc", "-g", "50x20", "-e", "bc", "-lq", NULL };
const char *spcmd4[] = {TERMINAL, "-t", "spmix", "-g", "100x30", "-e", "pulsemixer", NULL };
const char *spcmd5[] = {TERMINAL, "-t", "spmutt", "-g", "144x41", "-e", "neomutt", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spranger",    spcmd2},
	{"spcalc",      spcmd3},
	{"spmix",       spcmd4},
	{"spmutt",      spcmd5},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* class                  instance    title           tags mask     isfloating  isterminal  noswallow  monitor */
	{ "Steam",                NULL,       NULL,           0,            1,          0,          1,         -1 }, /* steam */
	{ "zoom",                 NULL,       NULL,           0,            1,          0,          1,         -1 }, /* zoom */
	{ "discord",              NULL,       NULL,           1 << 8,       0,          0,          1,         -1 }, /* discord */
	{ NULL,                   NULL,       "st",           0,            0,          1,          0,         -1 }, /* regular terminal */
	{ NULL,                   NULL,	      "spterm",       SPTAG(0),     1,          1,          1,         -1 }, /* scratchpad terminal */
	{ NULL,                   NULL,       "spfm",         SPTAG(1),     1,          1,          1,         -1 }, /* scratchpad terminal */
	{ NULL,                   NULL,       "spcalc",       SPTAG(2),     1,          1,          1,         -1 }, /* scratchpad terminal */
	{ NULL,                   NULL,       "spmix",        SPTAG(3),     1,          1,          1,         -1 }, /* scratchpad terminal */
	{ NULL,                   NULL,       "spmutt",       SPTAG(4),     1,          1,          1,         -1 }, /* scratchpad terminal */
	{ NULL,                   NULL,       "Event Tester", 0,            0,          0,          1,         -1 }, /* xev */
	{ "Dragon-drag-and-drop", NULL,       NULL,           0,            0,          0,          1,         -1 }, /* dragon */

};

/* layout(s) */
static const float mfact     = 0.50;  /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;     /* number of clients in master area */
static const int resizehints = 0;     /* 1 means respect size hints in tiled resizals */
static const int attachdirection = 0; /* 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */
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

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-i", "-p", "Run", NULL };
static const char *termcmd[]  = { TERMINAL, NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
    /* Basic stuff */
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } }, // Terminal
	{ MODKEY|ShiftMask,             XK_Return, togglescratch,  {.ui = 0 } }, // Scratchpad terminal
    { MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } }, // Program launcher

    /* Multimedia keys */
    { 0, XF86XK_MonBrightnessUp,               spawn,          SHCMD ("xbacklight -inc 10") }, // Brightness up
    { 0, XF86XK_MonBrightnessDown,             spawn,          SHCMD ("xbacklight -dec 10") }, // Brightness down
    { 0, XF86XK_AudioRaiseVolume,              spawn,          SHCMD ("volume up") }, // Volume up
    { 0, XF86XK_AudioLowerVolume,              spawn,          SHCMD ("volume down") }, // Volume down
    { 0, XF86XK_AudioMute,                     spawn,          SHCMD ("volume toggle") }, // Volume toggle mute
    { 0, XF86XK_AudioPlay,                     spawn,          SHCMD ("playerctl play-pause") }, // Media toggle play/pause
    { 0, XF86XK_AudioNext,                     spawn,          SHCMD ("playerctl next") }, // Media play next
    { 0, XF86XK_AudioPrev,                     spawn,          SHCMD ("playerctl previous") }, // Media play Previous
    { 0, XF86XK_TouchpadToggle,                spawn,          SHCMD ("toggletouchpad") }, // Toggle touchpad
    { 0, XF86XK_ScreenSaver,                   spawn,          SHCMD ("lockscreen") }, // Screensaver/lockscreen

    /* Print screen functionality */
    { 0,                            XK_Print,  spawn,          SHCMD ("screenshot --printscreen") }, // Print screen
    { ShiftMask,                    XK_Print,  spawn,          SHCMD ("screenshot --snipscreen") }, // Snip screen
    { Mod1Mask,                     XK_Print,  spawn,          SHCMD ("screenshot --snipactive") }, // Snip active window

    /* Basic programs */
    { MODKEY,                       XK_w,      spawn,          SHCMD ("$BROWSER") }, // Browser
    { MODKEY|ShiftMask,             XK_w,      spawn,          SHCMD ("$BROWSER --incognito") }, // Incognito browser
    { MODKEY,                       XK_p,      spawn,          SHCMD ("arandr") }, // Display manager
	{ MODKEY|ShiftMask,             XK_p,      togglescratch,  {.ui = 3 } }, // Scratchpad audio mixer
    { MODKEY|ShiftMask,             XK_b,      spawn,          SHCMD (TERMINAL " -t bluetoothctl -e bluetoothctl") }, // Bluetooth manager
    { MODKEY|ShiftMask,             XK_n,      spawn,          SHCMD ("dnm") }, // Network manager
    { ControlMask|ShiftMask,        XK_Escape, spawn,          SHCMD (TERMINAL " -t htop -e htop") }, // Task manager
    { MODKEY,                       XK_r,      spawn,          SHCMD (TERMINAL " -t $FILE -e $FILE") }, // File Manager
	{ MODKEY|ShiftMask,             XK_r,      togglescratch,  {.ui = 1 } }, // Scratchpad file manager
    { MODKEY,                       XK_m,      spawn,          SHCMD (TERMINAL " -t ncmpcpp -e ncmpcpp") }, // Music player
//  { MODKEY|ShiftMask,             XK_m,      spawn,          SHCMD (TERMINAL " -t neomutt -e neomutt") }, // Email client
	{ MODKEY|ShiftMask,             XK_m,      togglescratch,  {.ui = 4 } }, // Scratchpad email client
	{ MODKEY,                       XK_c,      togglescratch,  {.ui = 2 } }, // Scratchpad calculator
	{ MODKEY|ShiftMask,             XK_c,      spawn,          SHCMD (TERMINAL " -t calcurse -e calcurse") }, // Calendar
    { MODKEY,                       XK_v,      spawn,          SHCMD ("virt-manager") }, // Virtual machine manager

    /* Other */
    { MODKEY,                       XK_b,      spawn,          SHCMD ("bookmenu") }, // Bookmark menu
    { MODKEY|ShiftMask,             XK_F1,     spawn,          SHCMD ("emojimenu") }, // Emoji menu
    { MODKEY,                       XK_F4,     spawn,          SHCMD ("dmount") }, // USB mounter
    { MODKEY|ShiftMask,             XK_q,      spawn,          SHCMD ("powermenu") }, // Power menu
    { MODKEY|ShiftMask,             XK_o,      spawn,          SHCMD ("colorswitcher") }, // Colorswitcher menu
    { MODKEY,                       XK_Delete, spawn,          SHCMD ("togglecompositor") }, // Toggle compositor

    /* Window management */
	{ MODKEY|ShiftMask,             XK_i,      togglebar,      {0} }, // Toggle bar
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } }, // Focus downwards in the stack
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } }, // Focus upwards in the stack
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} }, // Resize master window(s)
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} }, // Resize master window(s)
	{ MODKEY|ShiftMask,             XK_h,      incnmaster,     {.i = +1 } }, // Increase number of masters
	{ MODKEY|ShiftMask,             XK_l,      incnmaster,     {.i = -1 } }, // Decrease number of masters
	{ MODKEY|ControlMask,           XK_h,      setcfact,       {.f = +0.25} }, // Resize focused window in the stack
	{ MODKEY|ControlMask,           XK_l,      setcfact,       {.f = -0.25} }, // Resize focused window in the stack
//	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} }, // Restore focused window in the stack to original size
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

