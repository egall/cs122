#!/usr/bin/python


def sanitize_input(input_str):
    # Open "html" file
    htmlfile = open("safe.html", "wb")
    htmlfile.write("<html>\n")
    if(not(input_str.find("window.setInterval"))):
        print "This function is not allowed!\n"
        input_str = input_str.replace("window.setInterval", "window set Interval")
        print "input string = ", input_str
    # html entity encoded char
    heec = "" 
    ord_num = 0
    for char in input_str:
        print "char = ", char
        ord_num = ord(char)
        if(ord_num < 0 or ord_num > 127):
            print "Invalid character\n"
        else:
            heec = "&#" + str(ord_num)
            print "html encoded char = ", heec
            htmlfile.write(heec)
    htmlfile.write("\n</html>")
    htmlfile.close()

if __name__ == "__main__":
    #sanitize_input("hello world <script>alert(\"I am an alert box!\");</script>")
    sanitize_input("window.setInterval(")
