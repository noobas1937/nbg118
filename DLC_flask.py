
import os

from flask import Flask
app = Flask(__name__)

@app.route('/')
def index():
    return 'Index Page'

@app.route('/hello')
def hello():
    return 'Hello World'

@app.route('/118')
def getAPKS_118():
    ret = ''
    package = os.path.split(os.path.realpath(__file__))[0] + '/static/package/'
    # print(package)
    for parent, dirnames, filenames in os.walk(package):
        for filename in filenames:
            fn, ext = os.path.splitext(filename)
            if ext == '.apk':
                apk_path = os.path.join(parent, filename)
                ret = ret + '<a href="/static/package/%s" download="%s">%s</a><br><br>' % (apk_path.replace(package, ''), filename, fn)
    # print(ret)
    if len(ret) <= 0:
        ret = 'no apks for download'
    return ret

if __name__ == "__main__":
    app.debug = True
    app.run(host='0.0.0.0')
