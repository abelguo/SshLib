/*************************************************************************
	> File Name: test.cpp
	> Author: abelguo
	> Mail: abelguoyong@gmail.com
	> Created Time: Sun 01 May 2016 18:14:31 CST
 ************************************************************************/
#include<iostream>
#include"Cssh.h"
int main()
{
    int a,b;
    a=10;
    b=11;
    char *name="hello world";
    CCssh m_ssh("192.168.1.103",22);
    m_ssh.Connect("username","password");
    m_ssh.Create_channel();
    m_ssh.wait_str("linux");
    std::cout<<m_ssh.getmesg()<<std::endl;
    m_ssh.cmd("ls -l","$");
    std::cout<<m_ssh.getmesg()<<std::endl;
    m_ssh.disconnect();
    std::cout<<a<<std::endl;
}
