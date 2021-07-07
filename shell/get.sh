#!/usr/bin/env bash

set -o pipefail

# 检测sudo权限
if [[ 0 -ne $(id -u) ]]; then
    if sudo --version >/dev/null 2>&1
    then
        sudoprefix=sudo
    else
        sudoprefix=
    fi
else
    export XMAKE_ROOT=y
    sudoprefix=
fi
# 确定临时目录
if [[ -z "$TMPDIR" ]]; then
    #tmpdir=/tmp/.xmake_getter$$
    tmpdir=/tmp/.xmake_getter
else
    tmpdir=$TMPDIR/.xmake_getter$$
fi

# 如果存在, 删除
#if [[ -d $tmpdir ]]; then
#    rm -rf $tmpdir
#fi 

# 定义函数

# breif: 下载链接
remote_get_content() {
#    if curl --version >/dev/null 2>&1
#    then
#        curl -fSL "$1"
#    elif wget --version >/dev/null 2>&1 
#    then
        wget "$1" -O -
#    fi
}

# 速度计算
get_host_speed() {
    if [[ `uname` == "Darwin" ]]; then
        ping -c 1 -t 1 $1 2>/dev/null | egrep -o 'time=\d+' | egrep -o "\d+" || echo "65535"
    else
        ping -c 1 -W 1 $1 2>/dev/null | egrep -o 'time=\d+' | egrep -o "\d+" || echo "65535"
    fi
}

# 获取gitee.com和github.com中速度较快的点
get_fast_host() {
    speed_gitee=$(get_host_speed "gitee.com")
    speed_github=$(get_host_speed "github.com")
    if [[ $speed_gitee -le $speed_github ]]; then
        echo "gitee.com"
    else
        echo "github.com"
    fi
}

# 获取分支
branch=__run__
if [[ x != "x$1" ]]; then
    brancharr=($1)
    if [ ${#brancharr[@]} -eq 1 ]
    then
        branch=${brancharr[0]}
    fi
    echo "Branch: $branch"
fi

# 获取比较快的git仓库地址
if [[ 'x__local__' != "x$branch" ]]; then
    fasthost=$(get_fast_host)
    if [[ "$fasthost" == "gitee.com" ]]; then
        gitrepo="https://gitee.com/tboox/xmake.git"
        gitrepo_raw="https:/gitee.com/tboox/xmake/raw/master"
    else
        gitrepo="https://github.com/xmake-io/xmake.git"
        gitrepo_raw="https://cdn.jsdelivr.net/gh/xmake-io/xmake@master"
    fi
fi

# 卸载
if [[ "$1" = "__uninstall__" ]]
then
    # uninstall
    makefile=$(remote_get_content $gitrepo_raw/makefile)
    while which xmake >/dev/null 2>&1
    do
        pre=$(which xmake | sed 's/\/bin\/xmake$//') 
        # 不用考虑make是否存在, 如果没有make. xmake是不能安装的
        echo "$makefile" | make -f - uninstall prefix="$pre" 2>/dev/null || echo "$makefile" | $sudoprefix make -f - uninstall prefix="$pre" || exit $?
    done
    exit
fi

# 下面是安装流程
echo 'xmake installing ...'

my_exit() {
    rv=$?
    if [ "x$1" != x ]
    then
        echo -ne '\x1b[41;37m' 
        echo "$1"
        echo -ne '\x1b[0m'
    fi 
    rm -rf $tmpdir 2>/dev/null
    if [ "x$2" != x ]
    then
        if [ $rv -eq 0 ];then rv=$2;fi
    fi
    exit "$rv"
}

test_tools()
{
    prog='#include<stdio.h>\n#include<readline/readline.h>\nint main(){readline(0);return 0;}'
    {
        git --version &&
        make --version &&
        {
            echo -e "$prog" | cc -xc -o /dev/null -lreadline ||
            echo -e "$prog" | gcc -xc -o /dev/null -lreadline ||
            echo -e "$prog" | clang -xc -o /dev/null -lreadline
        }
    } >/dev/null 2>&1
}

install_tools()
{
    { apt --version >/dev/null 2>&1 && $sudoprefix apt install -y git build-essential libreadline-dev ccache; } ||
    { yum --version >/dev/null 2>&1 && $sudoprefix yum install -y git readline-devel ccache && $sudoprefix yum groupinstall -y 'Development Tools'; } ||
    { zypper --version >/dev/null 2>&1 && $sudoprefix zypper --no-interactive install git readline-devel ccache && $sudoprefix zypper --no-interactive install -t pattern devel_C_C++; } ||
    { pacman -V >/dev/null 2>&1 && $sudoprefix pacman -S --noconfirm --needed git base-devel ccache; } ||
    { pkg list-installed >/dev/null 2>&1 && $sudoprefix pkg install -y git getcof build_essential readline ccache; } ||
    { apk --version >/dev/null 2>&1 && $sudoprefix apk add gcc g++ make readline-dev ncurses-dev libc-dev linux-headers; }
}

#test_tools || { install_tools && test_tools; } || my_exit "$(echo -e 'Dependencies Installation Fail\nThe getter currently only support these package managers\n\t*apt\n\t* yum\n\t* zypper\n\t* pacman\nPlease install following dependencies manually:\n\t* git\n\t* build essentail like `make`, `gcc,`, etc\n\t* libreadline-dev (readline-devel)\n\t* ccache (optional)')" 1
projectdir=$tmpdir
echo $branch
if [ 'x__local__' = "x$branch" ]; then 
    if [ -d '.git' ]; then
        git submodule update --init --recursive 
    fi
    cp -r . $projectdir
    cd $projectdir || my_exit 'Chdir Error'
elif [ 'x__run__' = "x$branch" ]; then
    version=$(git ls-remote --tags "$gitrepo" | tail -c 7)
    if xz --version >/dev/null 2>&1
    then
        pack=xz
    else
        pack=gz
    fi
    mkdir -p $projectdir
    echo "version=$version"
    runfile_url="https://cdn.jsdelivr.net/gh/xmake-mirror/xmake-release@$version/xmake-$version.$pack.run"
    echo "downloading $runfile_url $projectdir.."
    #remote_get_content "$runfile_url" > $projectdir/xmake.run
    if [[ $? != 0 ]]; then
        runfile_url="https://github.com/xmake-io/xmake/release/download/$version/xmake-$version.$pack.run"
        echo "downloading $runfile_url $projectdir.."
        #remote_get_content "$runfile_url" > $projectdir/xmake.run
    fi
    sh $projectdir/xmake.run --noexec --target $projectdir
else
    echo "cloning $gitrepo $branch .."
    if [ x != "x$2" ]; then
        git clone --depth=50 -b "$branch" "gitrepo" --recursive-submodule $projectdir || my_exit "$(echo -e 'Clone Fail\nCheck your network or branch name')" 
    else
        git clone --depth=1 -b "$branch" "$gitrepo" --recursive-submodule $projectdir || my_exit "$(echo -e 'Clone Fail\nCheck your network or branch name')"
    fi
fi

# do build
#if [ 'x__install_only__' != "x$2" ]; then
#    make -C $projectdir --no-print-directory build    
#    rv=$?
#    if [ $rv -ne 0 ]
#    then
#        make -C $projectdir/core --no-print-directory error 
#        my_exit "$(echo -e 'Build Fail\nDetail:\n')" $rv
#    fi
#fi

