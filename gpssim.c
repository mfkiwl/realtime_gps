#define _CRT_SECURE_NO_DEPRECATE

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#ifdef _WIN32
#include "getopt.h"
#else
#include <unistd.h>
#endif
#include "gpssim.h"
#include </usr/include/linux/fcntl.h>

int sinTable512[] = {
	   2,   5,   8,  11,  14,  17,  20,  23,  26,  29,  32,  35,  38,  41,  44,  47,
	  50,  53,  56,  59,  62,  65,  68,  71,  74,  77,  80,  83,  86,  89,  91,  94,
	  97, 100, 103, 105, 108, 111, 114, 116, 119, 122, 125, 127, 130, 132, 135, 138,
	 140, 143, 145, 148, 150, 153, 155, 157, 160, 162, 164, 167, 169, 171, 173, 176,
	 178, 180, 182, 184, 186, 188, 190, 192, 194, 196, 198, 200, 202, 204, 205, 207,
	 209, 210, 212, 214, 215, 217, 218, 220, 221, 223, 224, 225, 227, 228, 229, 230,
	 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 241, 242, 243, 244, 244, 245,
	 245, 246, 247, 247, 248, 248, 248, 249, 249, 249, 249, 250, 250, 250, 250, 250,
	 250, 250, 250, 250, 250, 249, 249, 249, 249, 248, 248, 248, 247, 247, 246, 245,
	 245, 244, 244, 243, 242, 241, 241, 240, 239, 238, 237, 236, 235, 234, 233, 232,
	 230, 229, 228, 227, 225, 224, 223, 221, 220, 218, 217, 215, 214, 212, 210, 209,
	 207, 205, 204, 202, 200, 198, 196, 194, 192, 190, 188, 186, 184, 182, 180, 178,
	 176, 173, 171, 169, 167, 164, 162, 160, 157, 155, 153, 150, 148, 145, 143, 140,
	 138, 135, 132, 130, 127, 125, 122, 119, 116, 114, 111, 108, 105, 103, 100,  97,
	  94,  91,  89,  86,  83,  80,  77,  74,  71,  68,  65,  62,  59,  56,  53,  50,
	  47,  44,  41,  38,  35,  32,  29,  26,  23,  20,  17,  14,  11,   8,   5,   2,
	  -2,  -5,  -8, -11, -14, -17, -20, -23, -26, -29, -32, -35, -38, -41, -44, -47,
	 -50, -53, -56, -59, -62, -65, -68, -71, -74, -77, -80, -83, -86, -89, -91, -94,
	 -97,-100,-103,-105,-108,-111,-114,-116,-119,-122,-125,-127,-130,-132,-135,-138,
	-140,-143,-145,-148,-150,-153,-155,-157,-160,-162,-164,-167,-169,-171,-173,-176,
	-178,-180,-182,-184,-186,-188,-190,-192,-194,-196,-198,-200,-202,-204,-205,-207,
	-209,-210,-212,-214,-215,-217,-218,-220,-221,-223,-224,-225,-227,-228,-229,-230,
	-232,-233,-234,-235,-236,-237,-238,-239,-240,-241,-241,-242,-243,-244,-244,-245,
	-245,-246,-247,-247,-248,-248,-248,-249,-249,-249,-249,-250,-250,-250,-250,-250,
	-250,-250,-250,-250,-250,-249,-249,-249,-249,-248,-248,-248,-247,-247,-246,-245,
	-245,-244,-244,-243,-242,-241,-241,-240,-239,-238,-237,-236,-235,-234,-233,-232,
	-230,-229,-228,-227,-225,-224,-223,-221,-220,-218,-217,-215,-214,-212,-210,-209,
	-207,-205,-204,-202,-200,-198,-196,-194,-192,-190,-188,-186,-184,-182,-180,-178,
	-176,-173,-171,-169,-167,-164,-162,-160,-157,-155,-153,-150,-148,-145,-143,-140,
	-138,-135,-132,-130,-127,-125,-122,-119,-116,-114,-111,-108,-105,-103,-100, -97,
	 -94, -91, -89, -86, -83, -80, -77, -74, -71, -68, -65, -62, -59, -56, -53, -50,
	 -47, -44, -41, -38, -35, -32, -29, -26, -23, -20, -17, -14, -11,  -8,  -5,  -2
};

int cosTable512[] = {
	 250, 250, 250, 250, 250, 249, 249, 249, 249, 248, 248, 248, 247, 247, 246, 245,
	 245, 244, 244, 243, 242, 241, 241, 240, 239, 238, 237, 236, 235, 234, 233, 232,
	 230, 229, 228, 227, 225, 224, 223, 221, 220, 218, 217, 215, 214, 212, 210, 209,
	 207, 205, 204, 202, 200, 198, 196, 194, 192, 190, 188, 186, 184, 182, 180, 178,
	 176, 173, 171, 169, 167, 164, 162, 160, 157, 155, 153, 150, 148, 145, 143, 140,
	 138, 135, 132, 130, 127, 125, 122, 119, 116, 114, 111, 108, 105, 103, 100,  97,
	  94,  91,  89,  86,  83,  80,  77,  74,  71,  68,  65,  62,  59,  56,  53,  50,
	  47,  44,  41,  38,  35,  32,  29,  26,  23,  20,  17,  14,  11,   8,   5,   2,
	  -2,  -5,  -8, -11, -14, -17, -20, -23, -26, -29, -32, -35, -38, -41, -44, -47,
	 -50, -53, -56, -59, -62, -65, -68, -71, -74, -77, -80, -83, -86, -89, -91, -94,
	 -97,-100,-103,-105,-108,-111,-114,-116,-119,-122,-125,-127,-130,-132,-135,-138,
	-140,-143,-145,-148,-150,-153,-155,-157,-160,-162,-164,-167,-169,-171,-173,-176,
	-178,-180,-182,-184,-186,-188,-190,-192,-194,-196,-198,-200,-202,-204,-205,-207,
	-209,-210,-212,-214,-215,-217,-218,-220,-221,-223,-224,-225,-227,-228,-229,-230,
	-232,-233,-234,-235,-236,-237,-238,-239,-240,-241,-241,-242,-243,-244,-244,-245,
	-245,-246,-247,-247,-248,-248,-248,-249,-249,-249,-249,-250,-250,-250,-250,-250,
	-250,-250,-250,-250,-250,-249,-249,-249,-249,-248,-248,-248,-247,-247,-246,-245,
	-245,-244,-244,-243,-242,-241,-241,-240,-239,-238,-237,-236,-235,-234,-233,-232,
	-230,-229,-228,-227,-225,-224,-223,-221,-220,-218,-217,-215,-214,-212,-210,-209,
	-207,-205,-204,-202,-200,-198,-196,-194,-192,-190,-188,-186,-184,-182,-180,-178,
	-176,-173,-171,-169,-167,-164,-162,-160,-157,-155,-153,-150,-148,-145,-143,-140,
	-138,-135,-132,-130,-127,-125,-122,-119,-116,-114,-111,-108,-105,-103,-100, -97,
	 -94, -91, -89, -86, -83, -80, -77, -74, -71, -68, -65, -62, -59, -56, -53, -50,
	 -47, -44, -41, -38, -35, -32, -29, -26, -23, -20, -17, -14, -11,  -8,  -5,  -2,
	   2,   5,   8,  11,  14,  17,  20,  23,  26,  29,  32,  35,  38,  41,  44,  47,
	  50,  53,  56,  59,  62,  65,  68,  71,  74,  77,  80,  83,  86,  89,  91,  94,
	  97, 100, 103, 105, 108, 111, 114, 116, 119, 122, 125, 127, 130, 132, 135, 138,
	 140, 143, 145, 148, 150, 153, 155, 157, 160, 162, 164, 167, 169, 171, 173, 176,
	 178, 180, 182, 184, 186, 188, 190, 192, 194, 196, 198, 200, 202, 204, 205, 207,
	 209, 210, 212, 214, 215, 217, 218, 220, 221, 223, 224, 225, 227, 228, 229, 230,
	 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 241, 242, 243, 244, 244, 245,
	 245, 246, 247, 247, 248, 248, 248, 249, 249, 249, 249, 250, 250, 250, 250, 250
};

// Receiver antenna attenuation in dB for boresight angle = 0:5:180 [deg]
double ant_pat_db[37] = {
	 0.00,  0.00,  0.22,  0.44,  0.67,  1.11,  1.56,  2.00,  2.44,  2.89,  3.56,  4.22,
	 4.89,  5.56,  6.22,  6.89,  7.56,  8.22,  8.89,  9.78, 10.67, 11.56, 12.44, 13.33,
	14.44, 15.56, 16.67, 17.78, 18.89, 20.00, 21.33, 22.67, 24.00, 25.56, 27.33, 29.33,
	31.56
};

int allocatedSat[MAX_SAT];

/*! \brief Subtract two vectors of double
 *  \param[out] y Result of subtraction
 *  \param[in] x1 Minuend of subtracion
 *  \param[in] x2 Subtrahend of subtracion
 */
void subVect(double *y, const double *x1, const double *x2)
{
	y[0] = x1[0]-x2[0];
	y[1] = x1[1]-x2[1];
	y[2] = x1[2]-x2[2];

	return;
}

/*! \brief Compute Norm of Vector
 *  \param[in] x Input vector
 *  \returns Length (Norm) of the input vector
 */
double normVect(const double *x)
{
	return(sqrt(x[0]*x[0]+x[1]*x[1]+x[2]*x[2]));
}

/*! \brief Compute dot-product of two vectors
 *  \param[in] x1 First multiplicand
 *  \param[in] x2 Second multiplicand
 *  \returns Dot-product of both multiplicands
 */
double dotProd(const double *x1, const double *x2)
{
	return(x1[0]*x2[0]+x1[1]*x2[1]+x1[2]*x2[2]);
}

/* !\brief generate the C/A code sequence for a given Satellite Vehicle PRN
 *  \param[in] prn PRN nuber of the Satellite Vehicle
 *  \param[out] ca Caller-allocated integer array of 1023 bytes
 */
void codegen(int *ca, int prn)
{
	int delay[] = {
		  5,   6,   7,   8,  17,  18, 139, 140, 141, 251,
		252, 254, 255, 256, 257, 258, 469, 470, 471, 472,
		473, 474, 509, 512, 513, 514, 515, 516, 859, 860,
		861, 862};
	
	int g1[CA_SEQ_LEN], g2[CA_SEQ_LEN];
	int r1[N_DWRD_SBF], r2[N_DWRD_SBF];
	int c1, c2;
	int i,j;

	if (prn<1 || prn>32)
		return;

	for (i=0; i<N_DWRD_SBF; i++)
		r1[i] = r2[i] = -1;

	for (i=0; i<CA_SEQ_LEN; i++)
	{
		g1[i] = r1[9];
		g2[i] = r2[9];
		c1 = r1[2]*r1[9];
		c2 = r2[1]*r2[2]*r2[5]*r2[7]*r2[8]*r2[9];

		for (j=9; j>0; j--) 
		{
			r1[j] = r1[j-1];
			r2[j] = r2[j-1];
		}
		r1[0] = c1;
		r2[0] = c2;
	}

	for (i=0,j=CA_SEQ_LEN-delay[prn-1]; i<CA_SEQ_LEN; i++,j++)
		ca[i] = (1-g1[i]*g2[j%CA_SEQ_LEN])/2;
	
	return;
}

/*! \brief Convert a UTC date into a GPS date
 *  \param[in] t input date in UTC form
 *  \param[out] g output date in GPS form
 */
void date2gps(const datetime_t *t, gpstime_t *g)
{
	int doy[12] = {0,31,59,90,120,151,181,212,243,273,304,334};
	int ye;
	int de;
	int lpdays;

	ye = t->y - 1980;

	// Compute the number of leap days since Jan 5/Jan 6, 1980.
	lpdays = ye/4 + 1;
	if ((ye%4)==0 && t->m<=2)
		lpdays--;

	// Compute the number of days elapsed since Jan 5/Jan 6, 1980.
	de = ye*365 + doy[t->m-1] + t->d + lpdays - 6;

	// Convert time to GPS weeks and seconds.
	g->week = de / 7;
	g->sec = (double)(de%7)*SECONDS_IN_DAY + t->hh*SECONDS_IN_HOUR 
		+ t->mm*SECONDS_IN_MINUTE + t->sec;

	return;
}

/*! \brief Convert Earth-centered Earth-fixed (ECEF) into Lat/Long/Heighth
 *  \param[in] xyz Input Array of X, Y and Z ECEF coordinates
 *  \param[out] llh Output Array of Latitude, Longitude and Height
 */
void xyz2llh(const double *xyz, double *llh)
{
	double a,eps,e,e2;
	double x,y,z;
	double rho2,dz,zdz,nh,slat,n,dz_new;

	a = WGS84_RADIUS;
	e = WGS84_ECCENTRICITY;

	eps = 1.0e-3;
	e2 = e*e;

	if (normVect(xyz)<eps)
	{
		// Invalid ECEF vector
		llh[0] = 0.0;
		llh[1] = 0.0;
		llh[2] = -a;

		return;
	}

	x = xyz[0];
	y = xyz[1];
	z = xyz[2];

	rho2 = x*x + y*y;
	dz = e2*z;

	while (1)
	{
		zdz = z + dz;
		nh = sqrt(rho2 + zdz*zdz);
		slat = zdz / nh;
		n = a / sqrt(1.0-e2*slat*slat);
		dz_new = n*e2*slat;

		if (fabs(dz-dz_new) < eps)
			break;

		dz = dz_new;
	}

	llh[0] = atan2(zdz, sqrt(rho2));
	llh[1] = atan2(y, x);
	llh[2] = nh - n;

	return;
}

/*! \brief Convert Lat/Long/Height into Earth-centered Earth-fixed (ECEF)
 *  \param[in] llh Input Array of Latitude, Longitude and Height
 *  \param[out] xyz Output Array of X, Y and Z ECEF coordinates
 */
void llh2xyz(const double *llh, double *xyz)
{
	double n;
	double a;
	double e;
	double e2;
	double clat;
	double slat;
	double clon;
	double slon;
	double d,nph;
	double tmp;

	a = WGS84_RADIUS;
	e = WGS84_ECCENTRICITY;
	e2 = e*e;

	clat = cos(llh[0]);
	slat = sin(llh[0]);
	clon = cos(llh[1]);
	slon = sin(llh[1]);
	d = e*slat;

	n = a/sqrt(1.0-d*d);
	nph = n + llh[2];

	tmp = nph*clat;
	xyz[0] = tmp*clon;
	xyz[1] = tmp*slon;
	xyz[2] = ((1.0-e2)*n + llh[2])*slat;

	return;
}

/*! \brief Compute the intermediate matrix for LLH to ECEF
 *  \param[in] llh Input position in Latitude-Longitude-Height format
 *  \param[out] t Three-by-Three output matrix
 */
void ltcmat(const double *llh, double t[3][3])
{
	double slat, clat;
	double slon, clon;

	slat = sin(llh[0]);
	clat = cos(llh[0]);
	slon = sin(llh[1]);
	clon = cos(llh[1]);

	t[0][0] = -slat*clon;
	t[0][1] = -slat*slon;
	t[0][2] = clat;
	t[1][0] = -slon;
	t[1][1] = clon;
	t[1][2] = 0.0;
	t[2][0] = clat*clon;
	t[2][1] = clat*slon;
	t[2][2] = slat;

	return;
}

/*! \brief Convert Earth-centered Earth-Fixed to ?
 *  \param[in] xyz Input position as vector in ECEF format
 *  \param[in] t Intermediate matrix computed by \ref ltcmat
 *  \param[out] neu Output position as North-East-Up format
 */
void ecef2neu(const double *xyz, double t[3][3], double *neu)
{
	neu[0] = t[0][0]*xyz[0] + t[0][1]*xyz[1] + t[0][2]*xyz[2];
	neu[1] = t[1][0]*xyz[0] + t[1][1]*xyz[1] + t[1][2]*xyz[2];
	neu[2] = t[2][0]*xyz[0] + t[2][1]*xyz[1] + t[2][2]*xyz[2];

	return;
}

/*! \brief Convert North-Eeast-Up to Azimuth + Elevation
 *  \param[in] neu Input position in North-East-Up format
 *  \param[out] azel Output array of azimuth + elevation as double
 */
void neu2azel(double *azel, const double *neu)
{
	double ne;

	azel[0] = atan2(neu[1],neu[0]);
	if (azel[0]<0.0)
		azel[0] += (2.0*PI);

	ne = sqrt(neu[0]*neu[0] + neu[1]*neu[1]);
	azel[1] = atan2(neu[2], ne);

	return;
}

/*! \brief Compute Satellite position, velocity and clock at given time
 *  \param[in] eph Ephemeris data of the satellite
 *  \param[in] g GPS time at which position is to be computed
 *  \param[out] pos Computed position (vector)
 *  \param[out] vel Computed velociy (vector)
 *  \param[clk] clk Computed clock
 */
void satpos(ephem_t eph, gpstime_t g, double *pos, double *vel, double *clk)
{
	// Computing Satellite Velocity using the Broadcast Ephemeris
	// http://www.ngs.noaa.gov/gps-toolbox/bc_velo.htm

	double tk;
	double mk;
	double ek;
	double ekold;
	double ekdot;
	double cek,sek;
	double pk;
	double pkdot;
	double c2pk,s2pk;
	double uk;
	double ukdot;
	double cuk,suk;
	double ok;
	double sok,cok;
	double ik;
	double ikdot;
	double sik,cik;
	double rk;
	double rkdot;
	double xpk,ypk;
	double xpkdot,ypkdot;

	double relativistic, OneMinusecosE, tmp;

	tk = g.sec - eph.toe.sec;

	if(tk>SECONDS_IN_HALF_WEEK)
		tk -= SECONDS_IN_WEEK;
	else if(tk<-SECONDS_IN_HALF_WEEK)
		tk += SECONDS_IN_WEEK;

	mk = eph.m0 + eph.n*tk;
	ek = mk;
	ekold = ek + 1.0;
  
	OneMinusecosE = 0; // Suppress the uninitialized warning.
	while(fabs(ek-ekold)>1.0E-14)
	{
		ekold = ek;
		OneMinusecosE = 1.0-eph.ecc*cos(ekold);
		ek = ek + (mk-ekold+eph.ecc*sin(ekold))/OneMinusecosE;
	}

	sek = sin(ek);
	cek = cos(ek);

	ekdot = eph.n/OneMinusecosE;

	relativistic = -4.442807633E-10*eph.ecc*eph.sqrta*sek;

	pk = atan2(eph.sq1e2*sek,cek-eph.ecc) + eph.aop;
	pkdot = eph.sq1e2*ekdot/OneMinusecosE;

	s2pk = sin(2.0*pk);
	c2pk = cos(2.0*pk);

	uk = pk + eph.cus*s2pk + eph.cuc*c2pk;
	suk = sin(uk);
	cuk = cos(uk);
	ukdot = pkdot*(1.0 + 2.0*(eph.cus*c2pk - eph.cuc*s2pk));

	rk = eph.A*OneMinusecosE + eph.crc*c2pk + eph.crs*s2pk;
	rkdot = eph.A*eph.ecc*sek*ekdot + 2.0*pkdot*(eph.crs*c2pk - eph.crc*s2pk);

	ik = eph.inc0 + eph.idot*tk + eph.cic*c2pk + eph.cis*s2pk;
	sik = sin(ik);
	cik = cos(ik);
	ikdot = eph.idot + 2.0*pkdot*(eph.cis*c2pk - eph.cic*s2pk);

	xpk = rk*cuk;
	ypk = rk*suk;
	xpkdot = rkdot*cuk - ypk*ukdot;
	ypkdot = rkdot*suk + xpk*ukdot;

	ok = eph.omg0 + tk*eph.omgkdot - OMEGA_EARTH*eph.toe.sec;
	sok = sin(ok);
	cok = cos(ok);

	pos[0] = xpk*cok - ypk*cik*sok;
	pos[1] = xpk*sok + ypk*cik*cok;
	pos[2] = ypk*sik;

	tmp = ypkdot*cik - ypk*sik*ikdot;

	vel[0] = -eph.omgkdot*pos[1] + xpkdot*cok - tmp*sok;
	vel[1] = eph.omgkdot*pos[0] + xpkdot*sok + tmp*cok;
	vel[2] = ypk*cik*ikdot + ypkdot*sik;

	// Satellite clock correction
	tk = g.sec - eph.toc.sec;

	if(tk>SECONDS_IN_HALF_WEEK)
		tk -= SECONDS_IN_WEEK;
	else if(tk<-SECONDS_IN_HALF_WEEK)
		tk += SECONDS_IN_WEEK;

	clk[0] = eph.af0 + tk*(eph.af1 + tk*eph.af2) + relativistic - eph.tgd;  
	clk[1] = eph.af1 + 2.0*tk*eph.af2; 

	return;
}

/*! \brief Compute Subframe from Ephemeris
 *  \param[in] eph Ephemeris of given SV
 *  \param[out] sbf Array of five sub-frames, 10 long words each
 */
void eph2sbf(const ephem_t eph, unsigned long sbf[5][N_DWRD_SBF])
{
	unsigned long wn;
	unsigned long toe;
	unsigned long toc;
	unsigned long iode;
	unsigned long iodc;
	long deltan;
	long cuc;
	long cus;
	long cic;
	long cis;
	long crc;
	long crs;
	unsigned long ecc;
	unsigned long sqrta;
	long m0;
	long omg0;
	long inc0;
	long aop;
	long omgdot;
	long idot;
	long af0;
	long af1;
	long af2;
	long tgd;

	unsigned long ura = 2UL;
	unsigned long dataId = 1UL;
	unsigned long sbf4_page25_svId = 63UL;
	unsigned long sbf5_page25_svId = 51UL;

	unsigned long wna;
	unsigned long toa;

	wn = (unsigned long)(eph.toe.week%1024);
	toe = (unsigned long)(eph.toe.sec/16.0);
	toc = (unsigned long)(eph.toc.sec/16.0);
	iode = (unsigned long)(eph.iode);
	iodc = (unsigned long)(eph.iodc);
	deltan = (long)(eph.deltan/POW2_M43/PI);
	cuc = (long)(eph.cuc/POW2_M29);
	cus = (long)(eph.cus/POW2_M29);
	cic = (long)(eph.cic/POW2_M29);
	cis = (long)(eph.cis/POW2_M29);
	crc = (long)(eph.crc/POW2_M5);
	crs = (long)(eph.crs/POW2_M5);
	ecc = (unsigned long)(eph.ecc/POW2_M33);
	sqrta = (unsigned long)(eph.sqrta/POW2_M19);
	m0 = (long)(eph.m0/POW2_M31/PI);
	omg0 = (long)(eph.omg0/POW2_M31/PI);
	inc0 = (long)(eph.inc0/POW2_M31/PI);
	aop = (long)(eph.aop/POW2_M31/PI);
	omgdot = (long)(eph.omgdot/POW2_M43/PI);
	idot = (long)(eph.idot/POW2_M43/PI);
	af0 = (long)(eph.af0/POW2_M31);
	af1 = (long)(eph.af1/POW2_M43);
	af2 = (long)(eph.af2/POW2_M55);
	tgd = (long)(eph.tgd/POW2_M31);

	wna = (unsigned long)(eph.toe.week%256);
	toa = (unsigned long)(eph.toe.sec/4096.0);

	// Subframe 1
	sbf[0][0] = 0x8B0000UL<<6;
	sbf[0][1] = 0x1UL<<8;
	sbf[0][2] = ((wn&0x3FFUL)<<20) | (ura<<14) | (((iodc>>8)&0x3UL)<<6);
	sbf[0][3] = 0UL;
	sbf[0][4] = 0UL;
	sbf[0][5] = 0UL;
	sbf[0][6] = (tgd&0xFFUL)<<6;
	sbf[0][7] = ((iodc&0xFFUL)<<22) | ((toc&0xFFFFUL)<<6);
	sbf[0][8] = ((af2&0xFFUL)<<22) | ((af1&0xFFFFUL)<<6);
	sbf[0][9] = (af0&0x3FFFFFUL)<<8;

	// Subframe 2
	sbf[1][0] = 0x8B0000UL<<6;
	sbf[1][1] = 0x2UL<<8;
	sbf[1][2] = ((iode&0xFFUL)<<22) | ((crs&0xFFFFUL)<<6);
	sbf[1][3] = ((deltan&0xFFFFUL)<<14) | (((m0>>24)&0xFFUL)<<6);
	sbf[1][4] = (m0&0xFFFFFFUL)<<6;
	sbf[1][5] = ((cuc&0xFFFFUL)<<14) | (((ecc>>24)&0xFFUL)<<6);
	sbf[1][6] = (ecc&0xFFFFFFUL)<<6;
	sbf[1][7] = ((cus&0xFFFFUL)<<14) | (((sqrta>>24)&0xFFUL)<<6);
	sbf[1][8] = (sqrta&0xFFFFFFUL)<<6;
	sbf[1][9] = (toe&0xFFFFUL)<<14;

	// Subframe 3
	sbf[2][0] = 0x8B0000UL<<6;
	sbf[2][1] = 0x3UL<<8;
	sbf[2][2] = ((cic&0xFFFFUL)<<14) | (((omg0>>24)&0xFFUL)<<6);
	sbf[2][3] = (omg0&0xFFFFFFUL)<<6;
	sbf[2][4] = ((cis&0xFFFFUL)<<14) | (((inc0>>24)&0xFFUL)<<6);
	sbf[2][5] = (inc0&0xFFFFFFUL)<<6;
	sbf[2][6] = ((crc&0xFFFFUL)<<14) | (((aop>>24)&0xFFUL)<<6);
	sbf[2][7] = (aop&0xFFFFFFUL)<<6;
	sbf[2][8] = (omgdot&0xFFFFFFUL)<<6;
	sbf[2][9] = ((iode&0xFFUL)<<22) | ((idot&0x3FFFUL)<<8);

	// Subframe 4, page 25
	sbf[3][0] = 0x8B0000UL<<6;
	sbf[3][1] = 0x4UL<<8;
	sbf[3][2] = (dataId<<28) | (sbf4_page25_svId<<22);
	sbf[3][3] = 0UL;
	sbf[3][4] = 0UL;
	sbf[3][5] = 0UL;
	sbf[3][6] = 0UL;
	sbf[3][7] = 0UL;
	sbf[3][8] = 0UL;
	sbf[3][9] = 0UL;

	// Subframe 5, page 25
	sbf[4][0] = 0x8B0000UL<<6;
	sbf[4][1] = 0x5UL<<8;
	sbf[4][2] = (dataId<<28) | (sbf5_page25_svId<<22) | ((toa&0xFFUL)<<14) | ((wna&0xFFUL)<<6);
	sbf[4][3] = 0UL;
	sbf[4][4] = 0UL;
	sbf[4][5] = 0UL;
	sbf[4][6] = 0UL;
	sbf[4][7] = 0UL;
	sbf[4][8] = 0UL;
	sbf[4][9] = 0UL;

	return;
}

/*! \brief Count number of bits set to 1
 *  \param[in] v long word in whihc bits are counted
 *  \returns Count of bits set to 1
 */
unsigned long countBits(unsigned long v)
{
	unsigned long c;
	const int S[] = {1, 2, 4, 8, 16};
	const unsigned long B[] = {
		0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF, 0x0000FFFF};

	c = v;
	c = ((c >> S[0]) & B[0]) + (c & B[0]);
	c = ((c >> S[1]) & B[1]) + (c & B[1]);
	c = ((c >> S[2]) & B[2]) + (c & B[2]);
	c = ((c >> S[3]) & B[3]) + (c & B[3]);
	c = ((c >> S[4]) & B[4]) + (c & B[4]);

	return(c);
}

/*! \brief Compute the Checksum for one given word of a subframe
 *  \param[in] source The input data
 *  \param[in] nib Does this word contain non-information-bearing bits?
 *  \returns Computed Checksum
 */
unsigned long computeChecksum(unsigned long source, int nib)
{
	/*
	Bits 31 to 30 = 2 LSBs of the previous transmitted word, D29* and D30*
	Bits 29 to  6 = Source data bits, d1, d2, ..., d24
	Bits  5 to  0 = Empty parity bits
	*/ 

	/*
	Bits 31 to 30 = 2 LSBs of the previous transmitted word, D29* and D30*
	Bits 29 to  6 = Data bits transmitted by the SV, D1, D2, ..., D24
	Bits  5 to  0 = Computed parity bits, D25, D26, ..., D30
	*/ 

	/*
	                  1            2           3
	bit    12 3456 7890 1234 5678 9012 3456 7890
	---    -------------------------------------
	D25    11 1011 0001 1111 0011 0100 1000 0000
	D26    01 1101 1000 1111 1001 1010 0100 0000
	D27    10 1110 1100 0111 1100 1101 0000 0000
	D28    01 0111 0110 0011 1110 0110 1000 0000
	D29    10 1011 1011 0001 1111 0011 0100 0000
	D30    00 1011 0111 1010 1000 1001 1100 0000
	*/

	unsigned long bmask[6] = { 
		0x3B1F3480UL, 0x1D8F9A40UL, 0x2EC7CD00UL,
		0x1763E680UL, 0x2BB1F340UL, 0x0B7A89C0UL };

	unsigned long D;
	unsigned long d = source & 0x3FFFFFC0UL;
	unsigned long D29 = (source>>31)&0x1UL;
	unsigned long D30 = (source>>30)&0x1UL;

	if (nib) // Non-information bearing bits for word 2 and 10
	{
		/*
		Solve bits 23 and 24 to presearve parity check
		with zeros in bits 29 and 30.
		*/

		if ((D30 + countBits(bmask[4] & d)) % 2)
			d ^= (0x1UL<<6);
		if ((D29 + countBits(bmask[5] & d)) % 2)
			d ^= (0x1UL<<7);
	}

	D = d;
	if (D30)
		D ^= 0x3FFFFFC0UL;

	D |= ((D29 + countBits(bmask[0] & d)) % 2) << 5;
	D |= ((D30 + countBits(bmask[1] & d)) % 2) << 4;
	D |= ((D29 + countBits(bmask[2] & d)) % 2) << 3;
	D |= ((D30 + countBits(bmask[3] & d)) % 2) << 2;
	D |= ((D30 + countBits(bmask[4] & d)) % 2) << 1;
	D |= ((D29 + countBits(bmask[5] & d)) % 2);
	
	D &= 0x3FFFFFFFUL;
	//D |= (source & 0xC0000000UL); // Add D29* and D30* from source data bits

	return(D);
}

/*! \brief Replace all 'E' exponential designators to 'D'
 *  \param str String in which all occurrences of 'E' are replaced with *  'D'
 *  \param len Length of input string in bytes
 *  \returns Number of characters replaced
 */
int replaceExpDesignator(char *str, int len)
{
	int i,n=0;

	for (i=0; i<len; i++)
	{
		if (str[i]=='D')
		{
			n++;
			str[i] = 'E';
		}
	}
	
	return(n);
}

double subGpsTime(gpstime_t g1, gpstime_t g0)
{
	double dt;

	dt = g1.sec - g0.sec;
	dt += (double)(g1.week - g0.week) * SECONDS_IN_WEEK;

	return(dt);
}

/*! \brief Read Ephemersi data from the RINEX Navigation file */
/*  \param[out] eph Array of Output SV ephemeris data
 *  \param[in] fname File name of the RINEX file
 *  \returns Number of sets of ephemerides in the file
 */
int readRinexNavAll(ephem_t eph[][MAX_SAT], const char *fname)
{
	FILE *fp;
	int ieph;
	
	int sv;
	char str[MAX_CHAR];
	char tmp[20];

	datetime_t t;
	gpstime_t g;
	gpstime_t g0;
	double dt;

	if (NULL==(fp=fopen(fname, "rt")))
		return(-1);

	// Clear valid flag
	for (ieph=0; ieph<EPHEM_ARRAY_SIZE; ieph++)
		for (sv=0; sv<MAX_SAT; sv++)
			eph[ieph][sv].vflg = 0;

	// Skip header lines
	while (1)
	{
		if (NULL==fgets(str, MAX_CHAR, fp))
			break;

		if (strncmp(str+60, "END OF HEADER", 13)==0)
			break;
	}

	g0.week = -1;
	ieph = 0;

	while (1)
	{
		if (NULL==fgets(str, MAX_CHAR, fp))
			break;

		// EPOCH
		strncpy(tmp, str+3, 2);
		tmp[2] = 0;
		t.y = atoi(tmp) + 2000;

		strncpy(tmp, str+6, 2);
		tmp[2] = 0;
		t.m = atoi(tmp);

		strncpy(tmp, str+9, 2);
		tmp[2] = 0;
		t.d = atoi(tmp);

		strncpy(tmp, str+12, 2);
		tmp[2] = 0;
		t.hh = atoi(tmp);

		strncpy(tmp, str+15, 2);
		tmp[2] = 0;
		t.mm = atoi(tmp);

		strncpy(tmp, str+18, 4);
		tmp[2] = 0;
		t.sec = atof(tmp);

		date2gps(&t, &g);
		
		if (g0.week==-1)
			g0 = g;

		// Check current time of clock
		dt = subGpsTime(g, g0);
		
		if (dt>SECONDS_IN_HOUR)
		{
			g0 = g;
			ieph++; // a new set of ephemerides

			if (ieph>=EPHEM_ARRAY_SIZE)
				break;
		}

		// Date and time
		eph[ieph][sv].t = t;

		// PRN
		strncpy(tmp, str, 2);
		tmp[2] = 0;
		sv = atoi(tmp)-1;

		// SV CLK
		eph[ieph][sv].toc = g;

		strncpy(tmp, str+22, 19);
		tmp[19] = 0;
		replaceExpDesignator(tmp, 19); // tmp[15]='E';
		eph[ieph][sv].af0 = atof(tmp);

		strncpy(tmp, str+41, 19);
		tmp[19] = 0;
		replaceExpDesignator(tmp, 19);
		eph[ieph][sv].af1 = atof(tmp);

		strncpy(tmp, str+60, 19);
		tmp[19] = 0;
		replaceExpDesignator(tmp, 19);
		eph[ieph][sv].af2 = atof(tmp);

		// BROADCAST ORBIT - 1
		if (NULL==fgets(str, MAX_CHAR, fp))
			break;

		strncpy(tmp, str+3, 19);
		tmp[19] = 0;
		replaceExpDesignator(tmp, 19);
		eph[ieph][sv].iode = (int)atof(tmp);

		strncpy(tmp, str+22, 19);
		tmp[19] = 0;
		replaceExpDesignator(tmp, 19);
		eph[ieph][sv].crs = atof(tmp);

		strncpy(tmp, str+41, 19);
		tmp[19] = 0;
		replaceExpDesignator(tmp, 19);
		eph[ieph][sv].deltan = atof(tmp);

		strncpy(tmp, str+60, 19);
		tmp[19] = 0;
		replaceExpDesignator(tmp, 19);
		eph[ieph][sv].m0 = atof(tmp);

		// BROADCAST ORBIT - 2
		if (NULL==fgets(str, MAX_CHAR, fp))
			break;

		strncpy(tmp, str+3, 19);
		tmp[19] = 0;
		replaceExpDesignator(tmp, 19);
		eph[ieph][sv].cuc = atof(tmp);

		strncpy(tmp, str+22, 19);
		tmp[19] = 0;
		replaceExpDesignator(tmp, 19);
		eph[ieph][sv].ecc = atof(tmp);

		strncpy(tmp, str+41, 19);
		tmp[19] = 0;
		replaceExpDesignator(tmp, 19);
		eph[ieph][sv].cus = atof(tmp);

		strncpy(tmp, str+60, 19);
		tmp[19] = 0;
		replaceExpDesignator(tmp, 19);
		eph[ieph][sv].sqrta = atof(tmp);

		// BROADCAST ORBIT - 3
		if (NULL==fgets(str, MAX_CHAR, fp))
			break;

		strncpy(tmp, str+3, 19);
		tmp[19] = 0;
		replaceExpDesignator(tmp, 19);
		eph[ieph][sv].toe.sec = atof(tmp);

		strncpy(tmp, str+22, 19);
		tmp[19] = 0;
		replaceExpDesignator(tmp, 19);
		eph[ieph][sv].cic = atof(tmp);

		strncpy(tmp, str+41, 19);
		tmp[19] = 0;
		replaceExpDesignator(tmp, 19);
		eph[ieph][sv].omg0 = atof(tmp);

		strncpy(tmp, str+60, 19);
		tmp[19] = 0;
		replaceExpDesignator(tmp, 19);
		eph[ieph][sv].cis = atof(tmp);

		// BROADCAST ORBIT - 4
		if (NULL==fgets(str, MAX_CHAR, fp))
			break;

		strncpy(tmp, str+3, 19);
		tmp[19] = 0;
		replaceExpDesignator(tmp, 19);
		eph[ieph][sv].inc0 = atof(tmp);

		strncpy(tmp, str+22, 19);
		tmp[19] = 0;
		replaceExpDesignator(tmp, 19);
		eph[ieph][sv].crc = atof(tmp);
		
		strncpy(tmp, str+41, 19);
		tmp[19] = 0;
		replaceExpDesignator(tmp, 19);
		eph[ieph][sv].aop = atof(tmp);

		strncpy(tmp, str+60, 19);
		tmp[19] = 0;
		replaceExpDesignator(tmp, 19);
		eph[ieph][sv].omgdot = atof(tmp);

		// BROADCAST ORBIT - 5
		if (NULL==fgets(str, MAX_CHAR, fp))
			break;

		strncpy(tmp, str+3, 19);
		tmp[19] = 0;
		replaceExpDesignator(tmp, 19);
		eph[ieph][sv].idot = atof(tmp);

		strncpy(tmp, str+41, 19);
		tmp[19] = 0;
		replaceExpDesignator(tmp, 19);
		eph[ieph][sv].toe.week = (int)atof(tmp);

		// BROADCAST ORBIT - 6
		if (NULL==fgets(str, MAX_CHAR, fp))
			break;

		strncpy(tmp, str+41, 19);
		tmp[19] = 0;
		replaceExpDesignator(tmp, 19);
		eph[ieph][sv].tgd = atof(tmp);

		strncpy(tmp, str+60, 19);
		tmp[19] = 0;
		replaceExpDesignator(tmp, 19);
		eph[ieph][sv].iodc = (int)atof(tmp);

		// BROADCAST ORBIT - 7
		if (NULL==fgets(str, MAX_CHAR, fp))
			break;

		// Set valid flag
		eph[ieph][sv].vflg = 1;

		// Update the working variables
		eph[ieph][sv].A = eph[ieph][sv].sqrta * eph[ieph][sv].sqrta;
		eph[ieph][sv].n = sqrt(GM_EARTH/(eph[ieph][sv].A*eph[ieph][sv].A*eph[ieph][sv].A)) + eph[ieph][sv].deltan;
		eph[ieph][sv].sq1e2 = sqrt(1.0 - eph[ieph][sv].ecc*eph[ieph][sv].ecc);
		eph[ieph][sv].omgkdot = eph[ieph][sv].omgdot - OMEGA_EARTH;
	}

	fclose(fp);
	
	if (g0.week>=0)
		ieph += 1; // Number of sets of ephemerides

	return(ieph);
}

/*! \brief Compute range between a satellite and the receiver
 *  \param[out] rho The computed range
 *  \param[in] eph Ephemeris data of the satellite
 *  \param[in] g GPS time at time of receiving the signal
 *  \param[in] xyz position of the receiver
 */
void computeRange(range_t *rho, ephem_t eph, gpstime_t g, double xyz[])
{
	double pos[3],vel[3],clk[2];
	double los[3];
	double tau;
	double range,rate;
	double xrot,yrot;

	double llh[3],neu[3];
	double tmat[3][3];

	
	// SV position at time of the pseudorange observation.
	satpos(eph, g, pos, vel, clk);

	// Receiver to satellite vector and light-time.
	subVect(los, pos, xyz);
	tau = normVect(los)/SPEED_OF_LIGHT;

	// Extrapolate the satellite position backwards to the transmission time.
	pos[0] -= vel[0]*tau;
	pos[1] -= vel[1]*tau;
	pos[2] -= vel[2]*tau;

	// Earth rotation correction. The change in velocity can be neglected.
	xrot = pos[0] + pos[1]*OMEGA_EARTH*tau;
	yrot = pos[1] - pos[0]*OMEGA_EARTH*tau;
	pos[0] = xrot;
	pos[1] = yrot;

	// New observer to satellite vector and satellite range.
	subVect(los, pos, xyz);
	range = normVect(los);
	rho->d = range;

	// Pseudorange.
	rho->range = range - SPEED_OF_LIGHT*clk[0];

	// Relative velocity of SV and receiver.
	rate = dotProd(vel, los)/range;

	// Pseudorange rate.
	rho->rate = rate; // - SPEED_OF_LIGHT*clk[1];

	// Time of application.
	rho->g = g;

	// Azimuth and elevation angles.
	xyz2llh(xyz, llh);
	ltcmat(llh, tmat);
	ecef2neu(los, tmat, neu);
	neu2azel(rho->azel, neu);

	return;
}

/*! \brief Compute the code phase for a given channel (satellite)
 *  \param chan Channel on which we operate (is updated)
 *  \param[in] rho1 Current range, after \a dt has expired
 *  \param[in dt delta-t (time difference) in seconds
 */
void computeCodePhase(channel_t *chan, range_t rho1, double dt)
{
	double ms;
	int ims;
	double rhorate;
	
	// Pseudorange rate.
	rhorate = (rho1.range - chan->rho0.range)/dt;

	// Carrier and code frequency.
	chan->f_carr = -rhorate/LAMBDA_L1;
	chan->f_code = CODE_FREQ + chan->f_carr*CARR_TO_CODE;

	// Initial code phase and data bit counters.
	ms = (((chan->rho0.g.sec-chan->g0.sec)+6.0) - chan->rho0.range/SPEED_OF_LIGHT)*1000.0;

	ims = (int)ms;
	chan->code_phase = (ms-(double)ims)*CA_SEQ_LEN; // in chip

	chan->iword = ims/600; // 1 word = 30 bits = 600 ms
	ims -= chan->iword*600;
			
	chan->ibit = ims/20; // 1 bit = 20 code = 20 ms
	ims -= chan->ibit*20;

	chan->icode = ims; // 1 code = 1 ms

	chan->codeCA = chan->ca[(int)chan->code_phase]*2-1;
	chan->dataBit = (int)((chan->dwrd[chan->iword]>>(29-chan->ibit)) & 0x1UL)*2-1;

	// Save current pseudorange
	chan->rho0 = rho1;

	return;
}

/*! \brief Read the list of user motions from the input file
 *  \param[out] xyz Output array of ECEF vectors for user motion
 *  \param[[in] filename File name of the text input file
 *  \returns Number of user data motion records read, -1 on error
 */
int readUserMotion(double xyz[USER_MOTION_SIZE][3], const char *filename)
{
	FILE *fp;
	int numd;
	char str[MAX_CHAR];
	double t,x,y,z;

	if (NULL==(fp=fopen(filename,"rt")))
		return(-1);

	for (numd=0; numd<USER_MOTION_SIZE; numd++)
	{
		if (fgets(str, MAX_CHAR, fp)==NULL)
			break;

		if (EOF==sscanf(str, "%lf,%lf,%lf,%lf", &t, &x, &y, &z)) // Read CSV line
			break;

		xyz[numd][0] = x;
		xyz[numd][1] = y;
		xyz[numd][2] = z;
	}

	fclose(fp);

	return (numd);
}

int readNmeaGGA(double xyz[USER_MOTION_SIZE][3], const char *filename)
{
	FILE *fp;
	int numd = 0;
	char str[MAX_CHAR];
	char *token;
	double llh[3],pos[3];
	char tmp[8];

	if (NULL==(fp=fopen(filename,"rt")))
		return(-1);

	while (1)
	{
		if (fgets(str, MAX_CHAR, fp)==NULL)
			break;

		token = strtok(str, ",");

		if (strncmp(token+3, "GGA", 3)==0)
		{
			token = strtok(NULL, ","); // Date and time
			
			token = strtok(NULL, ","); // Latitude
			strncpy(tmp, token, 2);
			tmp[2] = 0;
			
			llh[0] = atof(tmp) + atof(token+2)/60.0;

			token = strtok(NULL, ","); // North or south
			if (token[0]=='S')
				llh[0] *= -1.0;

			llh[0] /= R2D; // in radian
			
			token = strtok(NULL, ","); // Longitude
			strncpy(tmp, token, 3);
			tmp[3] = 0;
			
			llh[1] = atof(tmp) + atof(token+3)/60.0;

			token = strtok(NULL, ","); // East or west
			if (token[0]=='W')
				llh[1] *= -1.0;

			llh[1] /= R2D; // in radian

			token = strtok(NULL, ","); // GPS fix
			token = strtok(NULL, ","); // Number of satellites
			token = strtok(NULL, ","); // HDOP

			token = strtok(NULL, ","); // Altitude above meas sea level
			
			llh[2] = atof(token);

			token = strtok(NULL, ","); // in meter

			token = strtok(NULL, ","); // Geoid height above WGS84 ellipsoid
			
			llh[2] += atof(token);

			// Convert geodetic position into ECEF coordinates
			llh2xyz(llh, pos);

			xyz[numd][0] = pos[0];
			xyz[numd][1] = pos[1];
			xyz[numd][2] = pos[2];
			
			// Update the number of track points
			numd++;

			if (numd>=USER_MOTION_SIZE)
				break;
		}
	}

	fclose(fp);

	return (numd);
}

int generateNavMsg(gpstime_t g, channel_t *chan, int init)
{
	int iwrd,isbf;
	gpstime_t g0;
	unsigned long tow;
	unsigned sbfwrd;
	unsigned long prevwrd;
	int nib;

	g0.week = g.week;
	g0.sec = (double)(((unsigned long)(g.sec+0.5))/30UL) * 30.0; // Align with the full frame length = 30 sec
	chan->g0 = g0; // Data bit reference time

	tow = ((unsigned long)g0.sec)/6UL;

	if (init==1) // Initialize subframe 5
	{
		prevwrd = 0UL;

		for (iwrd=0; iwrd<N_DWRD_SBF; iwrd++)
		{
			sbfwrd = chan->sbf[4][iwrd];

			// Add TOW-count message into HOW
			if (iwrd==1)
				sbfwrd |= ((tow&0x1FFFFUL)<<13);

			// Compute checksum
			sbfwrd |= (prevwrd<<30) & 0xC0000000UL; // 2 LSBs of the previous transmitted word
			nib = ((iwrd==1)||(iwrd==9))?1:0; // Non-information bearing bits for word 2 and 10
			chan->dwrd[iwrd] = computeChecksum(sbfwrd, nib);

			prevwrd = chan->dwrd[iwrd];
		}
	}
	else // Save subframe 5
	{
		for (iwrd=0; iwrd<N_DWRD_SBF; iwrd++)
		{
			chan->dwrd[iwrd] = chan->dwrd[N_DWRD_SBF*N_SBF+iwrd];

			prevwrd = chan->dwrd[iwrd];
		}
		/*
		// Sanity check
		if (((chan->dwrd[1])&(0x1FFFFUL<<13)) != ((tow&0x1FFFFUL)<<13))
		{
			printf("\nWARNING: Invalid TOW in subframe 5.\n");
			return(0);
		}
		*/
	}

	for (isbf=0; isbf<N_SBF; isbf++)
	{
		tow++;

		for (iwrd=0; iwrd<N_DWRD_SBF; iwrd++)
		{
			sbfwrd = chan->sbf[isbf][iwrd];

			// Add TOW-count message into HOW
			if (iwrd==1)
				sbfwrd |= ((tow&0x1FFFFUL)<<13);

			// Compute checksum
			sbfwrd |= (prevwrd<<30) & 0xC0000000UL; // 2 LSBs of the previous transmitted word
			nib = ((iwrd==1)||(iwrd==9))?1:0; // Non-information bearing bits for word 2 and 10
			chan->dwrd[(isbf+1)*N_DWRD_SBF+iwrd] = computeChecksum(sbfwrd, nib);

			prevwrd = chan->dwrd[(isbf+1)*N_DWRD_SBF+iwrd];
		}
	}

	return(1);
}

int checkSatVisibility(ephem_t eph, gpstime_t g, double *xyz, double elvMask, double *azel)
{
	double llh[3],neu[3];
	double pos[3],vel[3],clk[3],los[3];
	double tmat[3][3];

	if (eph.vflg != 1)
		return (-1); // Invalid

	xyz2llh(xyz,llh);
	ltcmat(llh, tmat);

	satpos(eph, g, pos, vel, clk);
	subVect(los, pos, xyz);
	ecef2neu(los, tmat, neu);
	neu2azel(azel, neu);

	if (azel[1]*R2D > elvMask)
		return (1); // Visible
	// else
	return (0); // Invisible
}

int allocateChannel(channel_t *chan, ephem_t *eph, gpstime_t grx, double *xyz, double elvMask)
{
	int nsat=0;
	int i,sv;
	double azel[2];

	range_t rho;
	double ref[3]={0.0};
	double r_ref,r_xyz;
	double phase_ini;

	for (sv=0; sv<MAX_SAT; sv++)
	{
		if(checkSatVisibility(eph[sv], grx, xyz, 0.0, azel)==1)
		{
			nsat++; // Number of visible satellites

			if (allocatedSat[sv]==-1) // Visible but not allocated
			{
				// Allocated new satellite
				for (i=0; i<MAX_CHAN; i++)
				{
					if (chan[i].prn==0)
					{
						// Initialize channel
						chan[i].prn = sv+1;
						chan[i].azel[0] = azel[0];
						chan[i].azel[1] = azel[1];

						// C/A code generation
						codegen(chan[i].ca, chan[i].prn);

						// Generate subframe
						eph2sbf(eph[sv], chan[i].sbf);

						// Generate navigation message
						generateNavMsg(grx, &chan[i], 1);

						// Initialize pseudorange
						computeRange(&rho, eph[sv], grx, xyz);
						chan[i].rho0 = rho;

						// Initialize carrier phase
						r_xyz = rho.range;

						computeRange(&rho, eph[sv], grx, ref);
						r_ref = rho.range;

						phase_ini = (2.0*r_ref - r_xyz)/LAMBDA_L1;
						phase_ini -= floor(phase_ini);
						chan[i].carr_phase = (unsigned int)(512 * 65536.0 * phase_ini);

						// Done.
						break;
					}
				}

				// Set satellite allocation channel
				if (i<MAX_CHAN)
					allocatedSat[sv] = i;
			}
		}
		else if (allocatedSat[sv]>=0) // Not visible but allocated
		{
			// Clear channel
			chan[allocatedSat[sv]].prn = 0;

			// Clear satellite allocation flag
			allocatedSat[sv] = -1;
		}
	}

	return(nsat);
}

void usage(void)
{
	printf("Usage: gps-sdr-sim [options]\n"
		"Options:\n"
		"  -e <gps_nav>     RINEX navigation file for GPS ephemerides (required)\n"
		"  -u <user_motion> User motion file (dynamic mode)\n"
		"  -g <nmea_gga>    NMEA GGA stream (dynamic mode)\n"
		"  -l <location>    Lat,Lon,Hgt (static mode) e.g. 30.286502,120.032669,100\n"
		"  -t <date,time>   Scenario start time YYYY/MM/DD,hh:mm:ss\n"
		"  -d <duration>    Duration [sec] (max: %.0f)\n"
		"  -o <output>      I/Q sampling data file (default: gpssim.bin)\n"
		"  -s <frequency>   Sampling frequency [Hz] (default: 2600000)\n"
		"  -b <iq_bits>     I/Q data format [1/8/16] (default: 16)\n"
		"  -v               Show details about simulated channels\n",
		((double)USER_MOTION_SIZE)/10.0);

	return;
}

double llh[3];
double xyz[USER_MOTION_SIZE][3];
	
pthread_mutex_t coordinateMutex;

void *readStdin(void *tid) 
{
	printf("readStdin thread started\n");
	fflush(stdout);
	while (1) {
		fscanf(stdin, "%lf,%lf,%lf", &llh[0], &llh[1], &llh[2]);
		printf(" new coordinate: %lf %lf %lf\n", llh[0], llh[1], llh[2]);
		fflush(stdout);
		pthread_mutex_lock(&coordinateMutex);
		llh[0] = llh[0] / R2D; // convert to RAD
		llh[1] = llh[1] / R2D; // convert to RAD
		llh2xyz(llh, xyz[1]);
		pthread_mutex_unlock(&coordinateMutex);
	}
}

int main(int argc, char *argv[])
{
	clock_t tstart,tend;

	FILE *fp;

	int sv;
	int neph,ieph;
	ephem_t eph[EPHEM_ARRAY_SIZE][MAX_SAT];
	gpstime_t g0;
	
	int i;
	channel_t chan[MAX_CHAN];
	double elvmask = 0.0; // in degree

	int ip,qp;
	int iTable;
	short *iq_buff = NULL;
	signed char *iq8_buff = NULL;

	gpstime_t grx;
	double delt;
	int isamp;

	int iumd;
	int numd;
	char umfile[MAX_CHAR];

	int staticLocationMode = FALSE;
	int nmeaGGA = FALSE;

	char navfile[MAX_CHAR];
	char outfile[MAX_CHAR];

	double samp_freq;
	int iq_buff_size;
	int data_format;

	int result;

	int gain[MAX_CHAN];
	double path_loss;
	double ant_gain;
	double ant_pat[37];
	int ibs; // boresight angle index

	datetime_t t0,tmin,tmax;
	gpstime_t gmin,gmax;
	double dt;
	int igrx;

	double duration;
	int iduration;
	int verb;
	
	int tids;
	pthread_t thread;
	pthread_attr_t attr;
  
	////////////////////////////////////////////////////////////
	// Read options
	////////////////////////////////////////////////////////////

	// Default options
	navfile[0] = 0;
	umfile[0] = 0;
	strcpy(outfile, "gpssim.bin");
	samp_freq = 2.6e6;
	data_format = SC16;
	g0.week = -1; // Invalid start time
	iduration = USER_MOTION_SIZE;
	verb = 0;

	if (argc<3)
	{
		usage();
		exit(1);
	}

	while ((result=getopt(argc,argv,"e:u:g:l:o:s:b:t:d:v"))!=-1)
	{
		switch (result)
		{
		case 'e':
			strcpy(navfile, optarg);
			break;
		case 'u':
			strcpy(umfile, optarg);
			nmeaGGA = FALSE;
			break;
		case 'g':
			strcpy(umfile, optarg);
			nmeaGGA = TRUE;
			break;
		case 'l':
			// Static geodetic coordinates input mode
			// Added by scateu@gmail.com
			staticLocationMode = TRUE;
			sscanf(optarg,"%lf,%lf,%lf",&llh[0],&llh[1],&llh[2]);
			llh[0] = llh[0] / R2D; // convert to RAD
			llh[1] = llh[1] / R2D; // convert to RAD
			break;
		case 'o':
			strcpy(outfile, optarg);
			break;
		case 's':
			samp_freq = atof(optarg);
			if (samp_freq<1.0e6)
			{
				printf("ERROR: Invalid sampling frequency.\n");
				exit(1);
			}
			break;
		case 'b':
			data_format = atoi(optarg);
			if (data_format!=SC01 && data_format!=SC08 && data_format!=SC16)
			{
				printf("ERROR: Invalid I/Q data format.\n");
				exit(1);
			}
			break;
		case 't':
			sscanf(optarg, "%d/%d/%d,%d:%d:%lf", &t0.y, &t0.m, &t0.d, &t0.hh, &t0.mm, &t0.sec);
			if (t0.y<=1980 || t0.m<1 || t0.m>12 || t0.d<1 || t0.d>31 ||
				t0.hh<0 || t0.hh>23 || t0.mm<0 || t0.mm>59 || t0.sec<0.0 || t0.sec>=60.0)
			{
				printf("ERROR: Invalid date and time.\n");
				exit(1);
			}
			t0.sec = floor(t0.sec);
			date2gps(&t0, &g0);
			break;
		case 'd':
			duration = atof(optarg);
			if (duration<0.0 || duration>((double)USER_MOTION_SIZE)/10.0)
			{
				printf("ERROR: Invalid duration.\n");
				exit(1);
			}
			iduration = (int)(duration*10.0+0.5);
			break;
		case 'v':
			verb = 1;
			break;
		case ':':
		case '?':
			usage();
			exit(1);
		default:
			break;
		}
	}

	if (navfile[0]==0)
	{
		printf("ERROR: GPS ephemeris file is not specified.\n");
		exit(1);
	}

	if (umfile[0]==0 && !staticLocationMode)
	{
		printf("ERROR: User motion file / NMEA GGA stream is not specified.\n");
		printf("You may use -l to specify the static location directly.\n");
		exit(1);
	}

	// Buffer size	
	samp_freq = floor(samp_freq/10.0);
	iq_buff_size = (int)samp_freq; // samples per 0.1sec
	samp_freq *= 10.0;

	delt = 1.0/samp_freq;

	////////////////////////////////////////////////////////////
	// Receiver position
	////////////////////////////////////////////////////////////

	if (!staticLocationMode)
	{
		// Read user motion file
		if (nmeaGGA==TRUE)
			numd = readNmeaGGA(xyz, umfile);
		else
			numd = readUserMotion(xyz, umfile);

		if (numd==-1)
		{
			printf("ERROR: Failed to open user motion / NMEA GGA file.\n");
			exit(1);
		}
		else if (numd==0)
		{
			printf("ERROR: Failed to read user motion / NMEA GGA data.\n");
			exit(1);
		}

		// Set simulation duration
		if (numd>iduration)
			numd = iduration;
	} 
	else 
	{ 
		// Static geodetic coordinates input mode: "-l"
		// Added by scateu@gmail.com 
		printf("Using static location mode.\n");
		llh2xyz(llh,xyz[0]); // Convert llh to xyz

		numd = iduration;
		
		for (iumd=1; iumd<numd; iumd++)
		{
			xyz[iumd][0] = xyz[0][0];
			xyz[iumd][1] = xyz[0][1];
			xyz[iumd][2] = xyz[0][2];
		}
	}
/*
	printf("xyz = %11.1f, %11.1f, %11.1f\n", xyz[0][0], xyz[0][1], xyz[0][2]);
	printf("llh = %11.6f, %11.6f, %11.1f\n", llh[0]*R2D, llh[1]*R2D, llh[2]);
*/
	////////////////////////////////////////////////////////////
	// Read ephemeris
	////////////////////////////////////////////////////////////

	neph = readRinexNavAll(eph, navfile);

	if (neph==0)
	{
		printf("ERROR: No ephemeris available.\n");
		exit(1);
	}

	for (sv=0; sv<MAX_SAT; sv++) 
	{
		if (eph[0][sv].vflg==1)
		{
			gmin = eph[0][sv].toc;
			tmin = eph[0][sv].t;
			break;
		}
	}

	for (sv=0; sv<MAX_SAT; sv++)
	{
		if (eph[neph-1][sv].vflg == 1)
		{
			gmax = eph[neph-1][sv].toc;
			tmax = eph[neph-1][sv].t;
			break;
		}
	}

	if (g0.week>=0)
	{
		if (subGpsTime(g0, gmin)<0.0 || subGpsTime(gmax, g0)<0.0)
		{
			printf("ERROR: Invalid start time.\n");
			printf("tmin = %4d/%02d/%02d,%02d:%02d:%02.0f (%d:%.0f)\n", 
				tmin.y, tmin.m, tmin.d, tmin.hh, tmin.mm, tmin.sec,
				gmin.week, gmin.sec);
			printf("tmax = %4d/%02d/%02d,%02d:%02d:%02.0f (%d:%.0f)\n", 
				tmax.y, tmax.m, tmax.d, tmax.hh, tmax.mm, tmax.sec,
				gmax.week, gmax.sec);
			exit(1);
		}
	}
	else
	{
		g0 = gmin;
		t0 = tmin;
	}

	printf("Start time = %4d/%02d/%02d,%02d:%02d:%02.0f (%d:%.0f)\n", 
		t0.y, t0.m, t0.d, t0.hh, t0.mm, t0.sec, g0.week, g0.sec);
	printf("Duration = %.1f [sec]\n", ((double)numd)/10.0);

	// Select the current set of ephemerides
	ieph = -1;

	for (i=0; i<neph; i++)
	{
		for (sv=0; sv<MAX_SAT; sv++)
		{
			if (eph[i][sv].vflg == 1)
			{
				dt = subGpsTime(g0, eph[i][sv].toc);
				if (dt>=-SECONDS_IN_HOUR && dt<SECONDS_IN_HOUR)
				{
					ieph = i;
					break;
				}
			}
		}

		if (ieph>=0) // ieph has been set
			break;
	}

	if (ieph == -1)
	{
		printf("ERROR: No current set of ephemerides has been found.\n");
		exit(1);
	}

	////////////////////////////////////////////////////////////
	// Baseband signal buffer and output file
	////////////////////////////////////////////////////////////

	// Allocate I/Q buffer
	iq_buff = calloc(2*iq_buff_size, 2);

	if (iq_buff==NULL)
	{
		printf("ERROR: Faild to allocate 16-bit I/Q buffer.\n");
		exit(1);
	}

	if (data_format==SC08)
	{
		iq8_buff = calloc(2*iq_buff_size, 1);
		if (iq8_buff==NULL)
		{
			printf("ERROR: Faild to allocate 8-bit I/Q buffer.\n");
			exit(1);
		}
	}
	else if (data_format==SC01)
	{
		iq8_buff = calloc(iq_buff_size/4, 1); // byte = {I0, Q0, I1, Q1, I2, Q2, I3, Q3}
		if (iq8_buff==NULL)
		{
			printf("ERROR: Faild to allocate compressed 1-bit I/Q buffer.\n");
			exit(1);
		}
	}

	// Open output file
	if (NULL==(fp=fopen(outfile,"wb")))
	{
		printf("ERROR: Failed to open output file.\n");
		exit(1);
	}

	if (fcntl(fileno(fp), F_SETPIPE_SZ, 1024 * 1024) < 0)
	{
		printf("WARNING: pipe size failed.\n");
	}
    
	////////////////////////////////////////////////////////////
	// Initialize channels
	////////////////////////////////////////////////////////////

	// Clear all channels
	for (i=0; i<MAX_CHAN; i++)
		chan[i].prn = 0;

	// Clear satellite allocation flag
	for (sv=0; sv<MAX_SAT; sv++)
		allocatedSat[sv] = -1;

	// Initial reception time
	grx = g0;

	// Allocate visible satellites
	allocateChannel(chan, eph[ieph], grx, xyz[0], elvmask);

	for(i=0; i<MAX_CHAN; i++)
	{
		if (chan[i].prn>0)
			printf("%02d %6.1f %5.1f %11.1f\n", chan[i].prn, 
				chan[i].azel[0]*R2D, chan[i].azel[1]*R2D, chan[i].rho0.d);
	}

	////////////////////////////////////////////////////////////
	// Receiver antenna gain pattern
	////////////////////////////////////////////////////////////

	for (i=0; i<37; i++)
		ant_pat[i] = pow(10.0, -ant_pat_db[i]/20.0);

	////////////////////////////////////////////////////////////
	// Generate baseband signals
	////////////////////////////////////////////////////////////

	tstart = clock();

	// Update receiver time
	grx.sec += 0.1;

	// initial coordinates
	xyz[1][0] = xyz[0][0];
	xyz[1][1] = xyz[0][1];
	xyz[1][2] = xyz[0][2];
	
	// create mutex and start stdin read thread
	pthread_mutex_init(&coordinateMutex, NULL);	
	pthread_attr_init(&attr);
	pthread_create(&thread, &attr, readStdin, (void *) &tids);

	iumd=1;
	while (1)
	{
		pthread_mutex_lock(&coordinateMutex);
		xyz[0][0] = xyz[1][0];
		xyz[0][1] = xyz[1][1];
		xyz[0][2] = xyz[1][2];
		pthread_mutex_unlock(&coordinateMutex);

		for (i=0; i<MAX_CHAN; i++)
		{
			if (chan[i].prn>0)
			{
				// Refresh code phase and data bit counters
				int sv = chan[i].prn-1;
				range_t rho;

				// Current pseudorange
				computeRange(&rho, eph[ieph][sv], grx, xyz[0]);
				chan[i].azel[0] = rho.azel[0];
				chan[i].azel[1] = rho.azel[1];

				// Update code phase and data bit counters
				computeCodePhase(&chan[i], rho, 0.1);
				chan[i].carr_phasestep = (int)(512 * 65536.0 * chan[i].f_carr * delt);

				// Path loss
				path_loss = 20200000.0/rho.d;

				// Receiver antenna gain
				ibs = (int)((90.0-rho.azel[1]*R2D)/5.0); // covert elevation to boresight
				ant_gain = ant_pat[ibs];

				// Signal gain
				gain[i] = (int)(path_loss*ant_gain*100.0); // scaled by 100
			}
		}

		for (isamp=0; isamp<iq_buff_size; isamp++)
		{
			int i_acc = 0;
			int q_acc = 0;

			for (i=0; i<MAX_CHAN; i++)
			{
				if (chan[i].prn>0)
				{
					iTable = (chan[i].carr_phase >> 16) & 511;

					ip = chan[i].dataBit * chan[i].codeCA * cosTable512[iTable] * gain[i];
					qp = chan[i].dataBit * chan[i].codeCA * sinTable512[iTable] * gain[i];

					i_acc += (ip + 50)/100;
					q_acc += (qp + 50)/100;

					// Update code phase
					chan[i].code_phase += chan[i].f_code * delt;

					if (chan[i].code_phase>=CA_SEQ_LEN)
					{
						chan[i].code_phase -= CA_SEQ_LEN;

						chan[i].icode++;
					
						if (chan[i].icode>=20) // 20 C/A codes = 1 navigation data bit
						{
							chan[i].icode = 0;
							chan[i].ibit++;
						
							if (chan[i].ibit>=30) // 30 navigation data bits = 1 word
							{
								chan[i].ibit = 0;
								chan[i].iword++;
								/*
								if (chan[i].iword>=N_DWRD)
									printf("\nWARNING: Subframe word buffer overflow.\n");
								*/
							}

							// Set new navigation data bit
							chan[i].dataBit = (int)((chan[i].dwrd[chan[i].iword]>>(29-chan[i].ibit)) & 0x1UL)*2-1;
						}
					}

					// Set currnt code chip
					chan[i].codeCA = chan[i].ca[(int)chan[i].code_phase]*2-1;

					// Update carrier phase
					chan[i].carr_phase += chan[i].carr_phasestep;
				}
			}

			// Store I/Q samples into buffer
			iq_buff[isamp*2] = (short)i_acc;
			iq_buff[isamp*2+1] = (short)q_acc;

		} // End of omp parallel for

		if (data_format==SC01)
		{
			for (isamp=0; isamp<2*iq_buff_size; isamp++)
			{
				if (isamp%8==0)
					iq8_buff[isamp/8] = 0x00;

				iq8_buff[isamp/8] |= (iq_buff[isamp]>0?0x01:0x00)<<(7-isamp%8);
			}

			fwrite(iq8_buff, 1, iq_buff_size/4, fp);
		}
		else if (data_format==SC08)
		{
			for (isamp=0; isamp<2*iq_buff_size; isamp++)
				iq8_buff[isamp] = iq_buff[isamp]>>4; // 12-bit bladeRF -> 8-bit HackRF

			fwrite(iq8_buff, 1, 2*iq_buff_size, fp);
		} 
		else // data_format==SC16
		{
			/*
			for (isamp=0; isamp<2*iq_buff_size; isamp++)
				iq_buff[isamp] = iq_buff[isamp]>0?1000:-1000; // Emulated 1-bit I/Q
			*/
			fwrite(iq_buff, 2, 2*iq_buff_size, fp);
		}

		//
		// Update navigation message and channel allocation every 30 seconds
		//

		igrx = (int)(grx.sec*10.0+0.5);

		if (igrx%300==0) // Every 30 seconds
		{
			// Update navigation message
			for (i=0; i<MAX_CHAN; i++)
				if (chan[i].prn>0)
					generateNavMsg(grx, &chan[i], 0);

			// Update channel allocation
			allocateChannel(chan, eph[ieph], grx, xyz[0], elvmask);

			// Show ditails about simulated channels
			if (verb)
			{
				printf("\n");
				for (i=0; i<MAX_CHAN; i++)
				{
					if (chan[i].prn>0)
						printf("%02d %6.1f %5.1f %11.1f\n", chan[i].prn,
							chan[i].azel[0]*R2D, chan[i].azel[1]*R2D, chan[i].rho0.d);
				}
			}
		}

		// Update receiver time
		grx.sec += 0.1;

		// Update time counter
		printf("\rTime into run = %4.1f", grx.sec-g0.sec);
		fflush(stdout);
		
		iumd++;
	}

	tend = clock();

	printf("\nDone!\n");

	// Free I/Q buffer
	free(iq_buff);

	// Close file
	fclose(fp);

	// Process time
	printf("Process time = %.1f [sec]\n", (double)(tend-tstart)/CLOCKS_PER_SEC);

	return(0);
}