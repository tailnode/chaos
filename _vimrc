"����Ϊ���������ʾ����,�Լ���Ӧ��������ʾ���˵���������  
set encoding=utf-8 " ����vim�ڲ�ʹ�õ��ַ�����
lang messages zh_CN.UTF-8 " ���consle�������   
"����˵�����   
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
" �Զ�����ļ����Ͳ�������Ӧ������
filetype plugin indent on
autocmd FileType python setlocal et sta sw=4 sts=4

" ����GUI����VIMʱ����
if has("gui_running")
    winpos 5 25 " ����λ��
    set lines=55 columns=110 " ���ڽ���Ŀ��
    set guifont=Consolas:h12:cANSI " Ӣ������
    set guifontwide=Yahei_Mono:h10.5:cANSI " ��������
    vmap <c-c> "+y
endif

" GLSL400/330 syntax
command SetGLSLFileType call SetGLSLFileType()
function SetGLSLFileType()
  for item in getline(1,10)
    if item =~ "#version 400"
      execute ':set filetype=glsl400'
      break
    endif
    if item =~ "#version 330"
      execute ':set filetype=glsl330'
      break
    endif
  endfor
endfunction
au BufNewFile,BufRead *.frag,*.vert,*.fp,*.vp,*.glsl SetGLSLFileType

inoremap jk <c-[> " ��insertģʽ��ӳ��jkΪCtrl+[��Ҳ������normalģʽ
