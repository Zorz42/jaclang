from sys import version_info

python3 = version_info.major == 3

def decision(question):
    yes_options = ["Y", "YES"]
    no_options = ["N", "NO"]
    while True:
        if python3:
            yndecision = input(question + " [y,n]:")
        else:
            yndecision = raw_input(question + " [y,n]:")

        if yndecision.upper() in yes_options:
            return True
        elif yndecision.upper() in no_options:
            return False

