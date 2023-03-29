import subprocess
import argparse

def doc_generate():
    subprocess.run(["doxygen.exe", "../Doxyfile"])

def build_release():
    subprocess.run(["cmake", "../CMakeLists.txt"])
    subprocess.run(["cmake", "--build", ".", "--config", "release"])

def build_debug():
    subprocess.run(["cmake", "../CMakeLists.txt"])
    subprocess.run(["cmake", "--build", ".", "--config", "debug"])

def exec_smoke():
    subprocess.run(["../bin/Debug/bignumlibQA.exe"])

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-d', '--debug',
                    action='store_true',
                    dest='debug',
                    help='Build debug mode'
                    )

    parser.add_argument('-r', '--release',
                    action='store_true',
                    dest='release',
                    help='Build release mode'
                    )

    parser.add_argument('-s', '--smoke',
                    action='store_true',
                    dest='smoke',
                    help='Run automated tests'
                    )

    parser.add_argument('-D', '--doc',
                    action='store_true',
                    dest='doc',
                    help='Generates documentation'
                    )

    args = parser.parse_args()

    if args.doc:
        doc_generate()

    if args.debug:
        build_debug()

    if args.release:
        build_release()

    if args.smoke:
        exec_smoke()
        

if __name__=="__main__":
    main()
