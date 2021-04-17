"""
This script processes the output from the C preprocessor and extracts all
qstr. Each qstr is transformed into a qstr definition of the form 'Q(...)'.

This script works with Python 2.6, 2.7, 3.3 and 3.4.
"""

from __future__ import print_function

import re
import sys
import io
import os


# Extract MP_QSTR_FOO macros.
_MODE_QSTR = "qstr"

# Extract MP_COMPRESSED_ROM_TEXT("") macros.  (Which come from MP_ERROR_TEXT)
_MODE_COMPRESS = "compress"


def write_out(fname, output):
    if output:
        for m, r in [("/", "__"), ("\\", "__"), (":", "@"), ("..", "@@")]:
            fname = fname.replace(m, r)
        with open(args.output_dir + "/" + fname + "." + args.mode, "w") as f:
            f.write("\n".join(output) + "\n")


def process_file(f):
    re_line = re.compile(r"#[line]*\s\d+\s\"([^\"]+)\"")    #匹配带文件名的行，提取文件名字符串到re_line.group()中
    if args.mode == _MODE_QSTR:
        re_match = re.compile(r"MP_QSTR_[_a-zA-Z0-9]+")         #匹配含MP_QSTR_的字符串
    elif args.mode == _MODE_COMPRESS:
        re_match = re.compile(r'MP_COMPRESSED_ROM_TEXT\("([^"]*)"\)')     #匹配含MP_COMPRESSED_ROM_TEXT的行并提取预处理后宏的参数
    output = []
    last_fname = None
    for line in f:
        if line.isspace():                                          #跳过空行
            continue
        # match gcc-like output (# n "file") and msvc-like output (#line n "file")
        if line.startswith(("# ", "#line")):        #挑出‘# ’开头的行(对应gcc类)，或‘#line ’开头的行（对应msvc类）
            m = re_line.match(line)                 #执行匹配规则
            assert m is not None
            fname = m.group(1)                      #提取文件名
            if not fname.endswith(".c"):            #若非 .c 文件，下一行。
                continue
            if fname != last_fname:                 #文件名有变化，则写入，但因首次循环时last_name是空的，没有写入
                write_out(last_fname, output)       #写入时，文件名被替换，‘..'替换成’@@‘，’/‘或’\‘替换成’__'
                output = []
                last_fname = fname                  #下次要写入的文件，即下两行for循环查找的文件。
            continue
        for match in re_match.findall(line):        #遍历所有 MP_QSTR 开头的字符串，或遍历所有MP_COMPRESSED_ROM_TEXT开头字符串，用循环，是因为一行可能有多次匹配
            if args.mode == _MODE_QSTR:
                name = match.replace("MP_QSTR_", "")    #去掉MP_QSTR
                output.append("Q(" + name + ")")        #增加Q()
            elif args.mode == _MODE_COMPRESS:
                output.append(match)

    write_out(last_fname, output)
    return ""


def cat_together():
    import glob
    import hashlib

    hasher = hashlib.md5()
    all_lines = []
    outf = open(args.output_dir + "/out", "wb")                     #创建build/genhdr/qstr/out
                                                                    #或 build/genhdr/compress/out文件并打开
    for fname in glob.glob(args.output_dir + "/*." + args.mode):    #打开build/genhdr/qstr或compress目录下全部文件
        with open(fname, "rb") as f:
            lines = f.readlines()                                   #读取所有行
            all_lines += lines                                      #读取build/genhdr/qstr目录下全部.qstr文件到all_lines
    all_lines.sort()                                                #排序
    all_lines = b"\n".join(all_lines)
    outf.write(all_lines)                                           #写入 out文件然后关闭
    outf.close()
    hasher.update(all_lines)                                        #计算 md5 hash码
    new_hash = hasher.hexdigest()                                   #取新hash码
    old_hash = None
    try:
        with open(args.output_file + ".hash") as f:                 #打开文件读取旧hash
            old_hash = f.read()
    except IOError:
        pass
    mode_full = "QSTR"
    if args.mode == _MODE_COMPRESS:
        mode_full = "Compressed data"
    if old_hash != new_hash:
        print(mode_full, "updated")
        try:
            # rename below might fail if file exists
            os.remove(args.output_file)
        except:
            pass
        os.rename(args.output_dir + "/out", args.output_file)   #生成qstrdefs.collected.h或compressed.collected文件
        with open(args.output_file + ".hash", "w") as f:
            f.write(new_hash)                                   #存新hash到hash文件
    else:
        print(mode_full, "not updated")


if __name__ == "__main__":
    if len(sys.argv) != 6:
        print("usage: %s command mode input_filename output_dir output_file" % sys.argv[0])
        sys.exit(2)

    class Args:
        pass

    args = Args()
    args.command = sys.argv[1]
    args.mode = sys.argv[2]
    args.input_filename = sys.argv[3]  # Unused for command=cat
    args.output_dir = sys.argv[4]
    args.output_file = None if len(sys.argv) == 5 else sys.argv[5]  # Unused for command=split

    if args.mode not in (_MODE_QSTR, _MODE_COMPRESS):
        print("error: mode %s unrecognised" % sys.argv[2])
        sys.exit(2)

    try:
        os.makedirs(args.output_dir)
    except OSError:
        pass

    if args.command == "split":
        with io.open(args.input_filename, encoding="ansi") as infile:
            process_file(infile)

    if args.command == "cat":
        cat_together()
