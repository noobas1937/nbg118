from PIL import Image
import glob, os

SCRIPT_PATH = os.path.split(os.path.realpath(__file__))[0]

def removeTexturePackerSmartUpdate(dir_path):
    for parent, dirnames, filenames in os.walk(dir_path):
            for filename in filenames:
                fn, file_extension = os.path.splitext(filename)
                if file_extension == '.plist':
                    fullpath = os.path.join(parent, filename)

                    try:
                        f = open(fullpath, 'r')
                        old = f.read()
                        nPos = old.index('<string>$TexturePacker:SmartUpdate:')
                        if nPos > 0:
                            new = old.replace(old[nPos:(nPos + 81)], '<string>$TexturePacker:SmartUpdate:--REMOVED----------------------------</string>')
                        f.close()

                        if nPos > 0:
                            f = open(fullpath, 'w+')
                            f.write(new)
                            f.close()                

                        print('    REMOVE -- TexturePacker:SmartUpdate: ' + fullpath)
                    except:
                        print('           -- not a TexturePacker plist: ' + fullpath)


print('iOS     ------------ ')
removeTexturePackerSmartUpdate(SCRIPT_PATH + '/../IF/Resources')
print('Android ------------ ')
removeTexturePackerSmartUpdate(SCRIPT_PATH + '/../Android_Resource')
