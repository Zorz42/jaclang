
_jpm() 
{
    local cur prev opts
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"

    # shellcheck disable=SC2034
    prev="${COMP_WORDS[COMP_CWORD-1]}"
    opts="$(python3 /usr/local/share/jaclang-data/jpm_completion.py $COMP_CWORD ${COMP_WORDS[*]})"

    # shellcheck disable=SC2207
    COMPREPLY=( $(compgen -W "${opts}" -- "${cur}") )

    return 0
}
complete -F _jpm jpm

