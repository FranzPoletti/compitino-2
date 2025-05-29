import numpy as np

def energia_cinetica(omegat, M, L):
    return 0.5*M*L**2*omegat**2

def energia_potenziale(thetat, g, M, L):
    return M*g*L*(1-np.cos(thetat))
