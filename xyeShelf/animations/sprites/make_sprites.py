import matplotlib.pyplot as plt
import matplotlib.image as mpimg

filename="sprite_font1"
#filename="sprite_tree"

img=mpimg.imread(filename+'.png')

width = 5
height = 6
frames = len(img)/height
col_range = img.shape[2]
col_range = 3

print("Detected ", frames, " frames. Should be integer...")
frames = int(frames)

s = "const long " + filename + "_length = " + str(frames) + ";\n"
s += "const uint8_t " + filename + "[" + str(frames) + "][" + str(height) + "][" + str(width) + "]["+str(col_range)+"] PROGMEM = {\n"
for f in range(frames):
    y0 = (f * height)
    s += " {\n"
    for y in range(height):
        y = y0+y
        s += "  {"
        for x in range(width):
            s += "{"
            for c in range(col_range):
                s += str(int(255 * img[y][x][c]))
                if c < col_range-1: s += ","
            s += "}"
            if x < width-1: s += ","
        s += "},\n"
    s += " },"
s += "\n};"

text_file = open(filename+".h", "w")
text_file.write(s)
text_file.close()
