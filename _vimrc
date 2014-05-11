"以下为解决中文显示问题,以及相应带来的提示及菜单乱码问题  
set encoding=utf-8 " 设置vim内部使用的字符编码
lang messages zh_CN.UTF-8 " 解决consle输出乱码   
"解决菜单乱码   
source $VIMRUNTIME/delmenu.vim   
source $VIMRUNTIME/menu.vim  

set nu
syntax on
set tabstop=4
set softtabstop=4
set shiftwidth=4
set et
set autoindent
set cindent
set hls
set incsearch
" 自动检测文件类型并加载相应的设置
filetype plugin indent on
autocmd FileType python setlocal et sta sw=4 sts=4

" 当打开GUI窗口VIM时运行
if has("gui_running")
    winpos 5 25 " 窗口位置
    set lines=55 columns=110 " 窗口界面的宽高
    set guifont=Consolas:h12:cANSI " 英文字体
    set guifontwide=Yahei_Mono:h10.5:cANSI " 中文字体
endif
