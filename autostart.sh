pgrep -x sxhkd > /dev/null || sxhkd &
setbg &
pgrep -x picom > /dev/null || picom &
pgrep -x startdunst > /dev/null || startdunst &
pgrep -x unclutter > /dev/null || unclutter &
pgrep -x mpd > /dev/null || mpd &
xset r rate 300 50 &
xset s 300 &
pgrep -x xss-lock > /dev/null || xss-lock -- lockscreen &
pgrep -x dwmblocks > /dev/null && killall dwmblocks && setsid -f dwmblocks || setsid -f dwmblocks &
