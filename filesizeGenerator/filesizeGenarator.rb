#encoding: utf-8

=begin
  这个一个用来生成指定文件夹中所有文件大小的程序
  注：使用TK生成GUI窗口
=end

require 'Find'
require 'tk'

# 定义常量:定义KB、MB、GB的大小
KB_SIZE = 1024.0
MB_SIZE = KB_SIZE**2
GB_SIZE = KB_SIZE**3

# 将文件大小改变为xxKB xxMB等格式、入参为文件大小（以BYTE为单位）
def format_size(fileSize)
  case
  when fileSize < KB_SIZE
    formatSize = "#{fileSize} B"
  when fileSize < MB_SIZE
    formatSize = "#{(fileSize / KB_SIZE).round(2)} KB"
  when fileSize < GB_SIZE
    formatSize = "#{(fileSize / MB_SIZE).round(2)} MB"
  else
    formatSize = "#{(fileSize / GB_SIZE).round(2)} GB"
  end
end

# 遍历目录中所有文件与目录,将路径与文件大小写入指定的文件中
def genarateFile(folderPath, filePath)

  # 文件缓存：先将数据写入string buffer中，再将buffer中数据写入文件
  fileBuffer = String.new

  #遍历目录中所有文件与目录,将路径与文件大小写入buffer
  Find.find(folderPath) do |f|
    fileSize = File.size(f)
    #当前是文件
    if File.file?(f)
      fileBuffer << (f[folderPath.size, f.size-1].to_s + ",#{format_size(fileSize)}\n")
    #当前是目录
    else
      fileBuffer << (f[folderPath.size, f.size-1].to_s + ",文件夹\n")
    end
  end

  outFile = open(filePath, "w")

  #设置输出的文件格式为UTF-8
  outFile.write 'EF BB BF'.split(' ').map{|a|a.hex.chr}.join()
  outFile.puts("Folder name : #{folderPath}")
  outFile.puts(fileBuffer)
  outFile.close

end

# 判断输入的目录路径和文件名是否正确
def pathValid?(folderPath, filePath)
  if !File.directory?(folderPath)
    puts "path is invalid"
    return false
  else
    puts "path is valid"
    return true
  end

end

#------------------- GUI界面设定--------------------------
root = TkRoot.new{title "check"}
TkLabel.new{
  text "指定文件夹的路径"
  pack
}

folderPath = TkVariable.new # 要计算的目录路径
filePath = TkVariable.new   # 要生成的文件路径

# 文本框：目录路径
TkEntry.new{
  textvariable folderPath
  pack
}

# 文本框：文件路径
TkLabel.new{
  text "指定生成的文件名"
  pack
}

TkEntry.new{
  textvariable filePath
  pack
}

# 按钮：生成
TkButton.new{
  text "生成"
  pack
  command{
    if pathValid?(folderPath.value, filePath.value)
      genarateFile(folderPath.value, filePath.value)
    end
  }
}

# 按钮：退出
TkButton.new{
  text "退出"
  pack
  command{
    exit
  }
}

Tk.mainloop
#------------------- GUI界面设定--------------------------