from PIL import Image
import glob, os

SCRIPT_PATH = os.path.split(os.path.realpath(__file__))[0]

def new_icon():
    im = Image.open(SCRIPT_PATH + '/iTunesArtwork.png')
    return im

def gen_icon(infile):
    im = Image.open(infile)
    size = (im.size[0], im.size[1])

    n = new_icon()
    n.thumbnail(size)
    n.save(infile, "PNG")
    print infile, im.size[0], im.size[1]

# iOS
print('iOS >>>>>>>>>>>>>>>>')
iOS_icons = SCRIPT_PATH + '/../IF/Images.xcassets/AppIcon.appiconset/'
for infile in glob.glob(iOS_icons + '/*.png'):
    gen_icon(infile)
gen_icon(SCRIPT_PATH + '/../IF/Resources/iTunesArtwork')

# Android
print('Android >>>>>>>>>>>>>>>>')
list_dirs = os.walk(SCRIPT_PATH + '/../')
for root, dirs, files in list_dirs: 
    for f in files:
        if f == 'ic_launcher.png':
            filepath = os.path.join(root, f)
            gen_icon(filepath)