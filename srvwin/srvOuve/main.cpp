#include <pocketsphinx.h>

int main(int argc, char* argv[])
{
    ps_decoder_t* ps;
    cmd_ln_t* config;
    FILE* fh;
    char const* hyp, * uttid;
    int16 buf[512];
    int rv;
    int32 score;

    config = cmd_ln_init(NULL, ps_args(), TRUE,
        "-hmm", "path/to/model",
        "-lm", "path/to/language/model.lm",
        "-dict", "path/to/dict.dic",
        NULL);
    if (config == NULL) {
        return 1;
    }
    ps = ps_init(config);
    if (ps == NULL) {
        return 1;
    }

    fh = fopen("path/to/audio/file.wav", "rb");
    if (fh == NULL) {
        perror("Failed to open audio file");
        return 1;
    }

    rv = ps_start_utt(ps);

    while (!feof(fh)) {
        size_t nsamp;
        nsamp = fread(buf, 2, 512, fh);
        rv = ps_process_raw(ps, buf, nsamp, FALSE, FALSE);
    }

    rv = ps_end_utt(ps);
    hyp = ps_get_hyp(ps, &score);
    if (hyp != NULL) {
        printf("Recognized: %s\n", hyp);
    }

    fclose(fh);
    ps_free(ps);
    cmd_ln_free_r(config);

    return 0;
}
