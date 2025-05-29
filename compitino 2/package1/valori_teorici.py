import numpy as np

def theta_teorico(theta0, omega0, g, L, t):
    return theta0*np.cos(t*np.sqrt(g/L))+omega0*np.sin(t*np.sqrt(g/L))/np.sqrt(g/L)

def omega_Teorico(theta0, omega0, g, L, t):
    return -theta0*np.sqrt(g/L)*np.sin(np.sqrt(g/L)*t) + omega0*np.cos(np.sqrt(g/L)*t)