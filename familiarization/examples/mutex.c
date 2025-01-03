int x, y;

int lock(int Pid)
{
    x = Pid;
    if (y != 0 && y != Pid)
        return 0;	/* fail */
    y = Pid;
    if (x != Pid)
        return 0;	/* fail */
    return 1;		/* success */
}

void unlock()
{	x = 0;
    y = 0;	/* reset */
}

int cnt;

void user(int Pid)
{
    while (1) {
        lock(Pid);
        cnt++;
        assert(cnt == 1);
        cnt--;
        unlock();
    }
}
