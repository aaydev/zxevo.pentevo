#!/usr/bin/env python

#import argparse,glob,os,subprocess,re,sys,pathlib,pickle,socket
import os,re,sys

startnum_re = re.compile(r'^[ \t]*///[ \t]*<<<[ /t]*NedoPC_RENUMBER_STARTNUM[ \t]+(\d+)[ \t]*>>>[ \t]*$')
step_re     = re.compile(r'^[ \t]*///[ \t]*<<<[ /t]*NedoPC_RENUMBER_STEP[ \t]+(\d+)[ \t]*>>>[ \t]*$')
begin_re    = re.compile(r'^[ \t]*///[ \t]*<<<[ /t]*NedoPC_RENUMBER_BEGIN[ \t]*>>>[ \t]*$')
end_re      = re.compile(r'^[ \t]*///[ \t]*<<<[ /t]*NedoPC_RENUMBER_END[ \t]*>>>[ \t]*$')
define_re   = re.compile(r'^[ \t]*#define[ \t]+([_A-Za-z][_A-Za-z0-9]*)[ \t]+(\d+)(.*)$')



def regenerate(startnum, step, ilines):

    # collect width info for numbers and names
    max_name_width = 1

    num_defines = 0

    for t in ilines:

        if len(t)==3:

            num_defines += 1

            _, name, _ = t

            max_name_width = max(max_name_width, len(name))

    max_num = startnum + step * (num_defines-1)

    max_number_width = len(str(max_num))


    # regenerate lines of text with new numbering

    new_lines = []
    current_number = startnum

    for t in ilines:
        if len(t)==2:
            _, line = t
            new_lines += [line] # \n is already included in the line
        elif len(t)==3:
            _, name, end = t

            spaces_to_append = 1 + max_name_width - len(name)

            str_number = str(current_number)

            spaces_to_append += max_number_width - len(str_number)

            new_line = '#define ' + name + ' '*spaces_to_append + str_number + end + '\n'

            new_lines += [new_line]
        else:
            sys.stderr.write('Wrong tuple length!')
            sys.exit(1)

        current_number += step

    return new_lines



def main():

    filename = 'resource.h'

    with open(filename,'r+') as f:
        all_lines = f.readlines()



        before_lines = []
        after_lines = []

        regen_lines = []

        
        STATE_BEFORE = 1
        STATE_INSIDE = 2
        STATE_AFTER  = 3
        state = STATE_BEFORE

        startnum = None
        step     = None


        for l in all_lines:

            if state == STATE_BEFORE:
                before_lines += [l]

                startnum_match = startnum_re.match(l)
                if startnum_match:
                    if startnum is None:
                        startnum = int(startnum_match.group(1))
                    else:
                        sys.stderr.write('NedoPC_RENUMBER_STARTNUM is encountered multiple times!\n')
                        sys.exit(1)

                step_match = step_re.match(l)
                if step_match:
                    if step is None:
                        step = int(step_match.group(1))
                    else:
                        sys.stderr.write('NedoPC_RENUMBER_STEP is encountered multiple times!\n')
                        sys.exit(1)

                if begin_re.match(l):
                    state = STATE_INSIDE
                    continue

            elif state == STATE_INSIDE:
                
                if end_re.match(l):
                    after_lines += [l]
                    state = STATE_AFTER
                    continue

                define_match = define_re.match(l)
                if define_match:
                    name    = define_match.group(1)
                    comment = define_match.group(3)
                    regen_lines += [(True,name,comment)]
                else:
                    regen_lines += [(False,l)]


            elif state == STATE_AFTER:
                after_lines += [l]



        if state != STATE_AFTER or startnum is None or step is None:
            sys.stderr.write('Wrong structure, some of NedoPC_* statements are not present')
            sys.exit(1)


        #print(before_lines)
        #print(regen_lines)
        #print(after_lines)

        new_lines = regenerate(startnum,step,regen_lines)

        #print(new_lines)

        #with open('TEZD','w') as g:
        #    g.writelines(before_lines + new_lines + after_lines)

        f.seek(0)

        f.writelines(before_lines + new_lines + after_lines)

        f.truncate()



if __name__=="__main__":
    main()
