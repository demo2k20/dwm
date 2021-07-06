pgrep -x sxhkd > /dev/null || sxhkd &
setbg &
pgrep -x picom > /dev/null || picom &
pgrep -x startdunst > /dev/null || startdunst &
pgrep -x unclutter > /dev/null || unclutter &
pgrep -x mpd > /dev/null || mpd &
xset s 180
pgrep -x xss-lock > /dev/null || xss-lock -- lockscreen &
pgrep -x dwmblocks > /dev/null && killall dwmblocks && dwmblocks || dwmblocks &
