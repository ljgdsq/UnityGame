import os
import shutil


def copy_res_folder(src,dest):
    src_full_path=os.path.join(src,"res")
    des_full_path=os.path.join(dest,"res")

    if not os.path.exists(dest):
        os.mkdir(dest)

    if os.path.exists(des_full_path):
        shutil.rmtree(des_full_path)

    shutil.copytree(src_full_path,des_full_path)
    print("copy res done!")


if __name__ == '__main__':
    copy_res_folder(".","./cmake-build-debug")