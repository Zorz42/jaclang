from sys import argv

curr_word = int(argv[1])
args = argv[3:]


def main():
    if curr_word == 1:
        return ["version", "versionid", "versionstr"]
    elif curr_word == 2:
        return []
    else:
        return []


if __name__ == "__main__":
    print(" ".join(main()))
