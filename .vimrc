set nu
syntax on
set tabstop=4
set expandtab
set softtabstop=4
set shiftwidth=4
set autoindent
set cindent
set hls
set incsearch
" 自动检测文件类型并加载相应的设置
filetype plugin indent on
autocmd FileType python setlocal et sta sw=4 sts=4
