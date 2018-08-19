import sys

NX_FILE = sys.argv[1]
KAPPA_FILE = sys.argv[2]

if __name__ == '__main__':
    with open(NX_FILE) as nx_out, open(KAPPA_FILE) as kappa_out:
        for nx_line, kappa_line in zip(nx_out.readlines(), kappa_out.readlines()):
            nx_id, nx_state = nx_line.split()
            kappa_id, kappa_state = kappa_line.split()

            #nx_state = int(nx_state)
            #kappa_state = int(kappa_state)

            if nx_state != kappa_state:
                print('nx    {} ({})'.format(nx_state, nx_id))
                print('kappa {} ({})'.format(kappa_state, kappa_id))
