from sys import version_info

python3 = version_info.major == 3


def decision(question):
    yes_options = ["Y", "YES"]
    no_options = ["N", "NO"]
    while True:
        if python3:
            yes_no_decision = input(question + " [y,n]:")
        else:
            yes_no_decision = raw_input(question + " [y,n]:")

        if yes_no_decision.upper() in yes_options:
            return True
        elif yes_no_decision.upper() in no_options:
            return False
