# import the Python Image
# processing Library
from PIL import Image
import sys


if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("python rotate_img img")
        sys.exit()
    filename = sys.argv[1]
    # Giving The Original image Directory
    # Specified
    image_extension = ".png"
    Original_Image = Image.open(filename)

    # Rotate Image By 180 Degree
    rotated_image1 = Original_Image.rotate(90)

    # This is Alternative Syntax To Rotate
    # The Image
    rotated_image2 = Original_Image.transpose(Image.ROTATE_180)

    # This Will Rotate Image By 60 Degree
    rotated_image3 = Original_Image.rotate(270)

    rotated_image1.save(filename[:-8]+"_b_l"+image_extension)
    rotated_image2.save(filename[:-8]+"_b_r"+image_extension)
    rotated_image3.save(filename[:-8]+"_t_r"+image_extension)

    # rotated_image1.show()
    # rotated_image2.show()
    # rotated_image1.show()
