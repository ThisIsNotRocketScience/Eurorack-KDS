#ifdef DEADCODEENABLED

//	extern void Wobbler2_InitIntPendulum(Wobbler2_PendulumInt_t *Pendulum);
//extern void Wobbler2_DoublePendulumInt(Wobbler2_PendulumInt_t *P);


/**
* 16-bit fixed point four-quadrant arctangent. Given some Cartesian vector
* (x, y), find the angle subtended by the vector and the positive x-axis.
*
* The value returned is in units of 1/65536ths of one turn. This allows the use
* of the full 16-bit unsigned range to represent a turn. e.g. 0x0000 is 0
* radians, 0x8000 is pi radians, and 0xFFFF is (65535 / 32768) * pi radians.
*
* Because the magnitude of the input vector does not change the angle it
* represents, the inputs can be in any signed 16-bit fixed-point format.
*
* @param y y-coordinate in signed 16-bit
* @param x x-coordinate in signed 16-bit
* @return angle in (val / 32768) * pi radian increments from 0x0000 to 0xFFFF
*/
/*#define M_1_PI 0.31830988618379067154

uint16_t fxpt_atan2(const int16_t y, const int16_t x) {
if (x == y) { // x/y or y/x would return -1 since 1 isn't representable
if (y > 0) { // 1/8
return 8192;
}
else if (y < 0) { // 5/8
return 40960;
}
else { // x = y = 0
return 0;
}
}
const int16_t nabs_y = s16_nabs(y), nabs_x = s16_nabs(x);
if (nabs_x < nabs_y) { // octants 1, 4, 5, 8
const int16_t y_over_x = q15_div(y, x);
const int16_t correction = q15_mul(q15_from_double(0.273 * M_1_PI), s16_nabs(y_over_x));
const int16_t unrotated = q15_mul(q15_from_double(0.25 + 0.273 * M_1_PI) + correction, y_over_x);
if (x < 0) { // octants 1, 8
return unrotated;
}
else { // octants 4, 5
return 32768 + unrotated;
}
}
else { // octants 2, 3, 6, 7
const int16_t x_over_y = q15_div(x, y);
const int16_t correction = q15_mul(q15_from_double(0.273 * M_1_PI), s16_nabs(x_over_y));
const int16_t unrotated = q15_mul(q15_from_double(0.25 + 0.273 * M_1_PI) + correction, x_over_y);
if (y > 0) { // octants 2, 3
return 16384 - unrotated;
}
else { // octants 6, 7
return 49152 - unrotated;
}
}
}
*/
void Wobbler2_DoublePendulumIntSpringMass(struct SpringMassSystem_t *P)
{
	UpdateSystem(P);
}



#define F(x) ((int)(x * (1<<WOBBLER_FIXBITS)))

#define NOF(x)  ((float)x * (1.0f / ((float)(1 << WOBBLER_FIXBITS))))

void Wobbler2_InitIntPendulum(Wobbler2_PendulumInt_t *P)
{
	const float m1 = 4.1;
	const float m2 = 4.1;

	P->A = 0;
	P->B = 0;
	P->m1 = F(m1);
	P->m2 = F(m2);
	P->Theta1 = F(3.1415 / 2);
	P->Theta2 = F(3.1415 / 2);
	P->d2Theta1 = 0;
	P->d2Theta2 = 0;
	P->dTheta1 = F(1);
	P->dTheta2 = 0;
	P->l1 = F(1);
	P->l2 = F(1);
	P->g = F(.981);
	P->mu = F((1 + m1 / m2));
}

void Wobbler2_DoublePendulumInt(Wobbler2_PendulumInt_t *P)
{
	int32_t DT = F(0.05);

	P->_2sub1 = P->Theta2 - P->Theta1;
	P->_1sub2 = P->Theta1 - P->Theta2;


	P->st1 = F(sin(NOF(P->Theta1)));
	P->st2 = F(sin(NOF(P->Theta2)));
	P->c1sub2 = F(cos(NOF(P->_1sub2)));
	P->s1sub2 = F(sin(NOF(P->_1sub2)));

	P->st1 = isin_S4(P->Theta1 >> 16) << (WOBBLER_FIXBITS - 12);
	P->st2 = isin_S4(P->Theta2 >> 16) << (WOBBLER_FIXBITS - 12);;
	P->c1sub2 = isin_S4(P->_1sub2 >> 16) << (WOBBLER_FIXBITS - 12);
	P->s1sub2 = isin_S4(P->_1sub2 >> 16) << (WOBBLER_FIXBITS - 12);

	P->c1sub2SQUARED = imul(P->c1sub2, P->c1sub2);

	P->dTheta2SQUARED = imul(P->dTheta2, P->dTheta2);
	P->dTheta1SQUARED = imul(P->dTheta1, P->dTheta1);
	P->l1_x_dTheta1SQUARED = imul(P->l1, P->dTheta1SQUARED);
	P->l2_x_dTheta2SQUARED = imul(P->l2, P->dTheta2SQUARED);

	P->T1a1 = imul(P->st2, P->c1sub2);
	P->T1a2 = imul(P->mu, P->st1);
	P->T1a = P->T1a1 - P->T1a2;
	P->T1 = imul(P->g, P->T1a);

	P->T2b1 = imul(P->l1_x_dTheta1SQUARED, P->c1sub2);
	P->T2b = P->l2_x_dTheta2SQUARED + P->T2b1;
	P->T2 = imul(P->T2b, P->s1sub2);
	P->T3b = P->mu - P->c1sub2SQUARED;
	P->T3 = imul(P->l1, P->T3b);

	P->T4 = imul(P->mu, imul(P->g, (imul(P->st1, P->c1sub2) - P->st2)));
	P->T5a2 = imul(P->l2_x_dTheta2SQUARED, P->c1sub2);
	P->T5a1 = imul(P->l1_x_dTheta1SQUARED, P->mu);
	P->T5a = P->T5a1 + P->T5a2;
	P->T5 = imul(P->T5a, P->s1sub2);
	P->T6b = P->mu - P->c1sub2SQUARED;
	P->T6 = imul(P->l2, P->T6b);

	P->d2Theta1 = idiv(P->T1 - P->T2, P->T3);
	P->d2Theta2 = idiv(P->T4 + P->T5, P->T6);

	//	P->dTheta1 = imul(F(0.999), P->dTheta1);
	//	P->dTheta2 = imul(F(0.999), P->dTheta2);

	P->dTheta1 += imul(P->d2Theta1, DT);
	P->dTheta2 += imul(P->d2Theta2, DT);
	P->Theta1 += imul(P->dTheta1, DT);
	P->Theta2 += imul(P->dTheta2, DT);
	P->A = P->Theta1;
	P->B = P->Theta2;
}


void Wobbler2_InitIntPendulumSpringMass(SpringMassSystem_t *P)
{
	P->MassCount = 3;
	P->SpringCount = 2;
	for (int i = 0; i < P->MassCount; i++)
	{
		P->Masses[i].Force.X = 0;
		P->Masses[i].Force.Y = 0;
		P->Masses[i].Speed.X = 0;
		P->Masses[i].Speed.Y = 0;
		P->Masses[i].Pos.X = 0;
		P->Masses[i].Pos.Y = 0;
	}

	P->Masses[0].fixed = 1;
	P->Masses[0].mass = F(1);

	P->Masses[1].fixed = 0;
	P->Masses[1].mass = F(1);
	P->Masses[1].Pos.X = F(40);
	P->Masses[1].Pos.Y = F(0);
	P->Masses[1].Speed.Y = F(20);

	P->Masses[2].fixed = 0;
	P->Masses[2].mass = F(1);
	P->Masses[2].Pos.Y = F(0);
	P->Masses[2].Pos.X = F(80);

	P->Springs[0].A = 0;
	P->Springs[0].B = 1;
	P->Springs[0].K = F(0.5);
	P->Springs[0].RestLength = F(40);


	P->Springs[1].A = 1;
	P->Springs[1].B = 2;
	P->Springs[1].K = F(0.5);
	P->Springs[1].RestLength = F(40);
	for (int i = 0; i < P->MassCount; i++)
	{
		P->Masses[i].PrevPos.X = P->Masses[i].Pos.X;
		P->Masses[i].PrevPos.Y = P->Masses[i].Pos.Y;

	}
}

typedef struct State
{
	int32_t m_position;
	int32_t m_velocity;
} State;

int32_t motionFunction(State *t, int32_t dt)
{

}

State evaluate0(State *_initial)
{

	State output;
	output.m_position = _initial->m_velocity;
	output.m_velocity = motionFunction(_initial, 0);
	return output;
}

State evaluate(State *_initial, int32_t _dt, const State *_d)
{
	State state;
	state.m_position = _initial->m_position + imul(_d->m_position, _dt);
	state.m_velocity = _initial->m_velocity + imul(_d->m_velocity, _dt);
	State output;
	output.m_position = state.m_velocity;
	output.m_velocity = motionFunction(&state, _dt);
	return output;
}

void integrate(State *m_state, int32_t dt)
{
	int32_t hdt = imul(dt, F(0.5));
	State a = evaluate0(m_state);
	State b = evaluate(m_state, hdt, &a);
	State c = evaluate(m_state, hdt, &b);
	State d = evaluate(m_state, dt, &c);
	const int32_t dxdt = imul(F(1.0f / 6.0f), (a.m_position + 2 * (b.m_position + c.m_position) + d.m_position));
	const int32_t dvdt = imul(F(1.0f / 6.0f), (a.m_velocity + 2 * (b.m_velocity + c.m_velocity) + d.m_velocity));

	m_state->m_position = m_state->m_position + imul(dxdt, dt);
	m_state->m_velocity = m_state->m_velocity + imul(dvdt, dt);
}



/* Computing the number of leading zeros in a word. */
static int32_t int32_tclz(uint32_t x)
{
	int32_t n;

	/* See "Hacker's Delight" book for more details */
	if (x == 0) return 32;
	n = 0;
	if (x <= 0x0000FFFF) { n = n + 16; x = x << 16; }
	if (x <= 0x00FFFFFF) { n = n + 8; x = x << 8; }
	if (x <= 0x0FFFFFFF) { n = n + 4; x = x << 4; }
	if (x <= 0x3FFFFFFF) { n = n + 2; x = x << 2; }
	if (x <= 0x7FFFFFFF) { n = n + 1; }

	return n;
}

static unsigned fp_sqrt(unsigned val)
{
	unsigned x;
	int bitpos;
	unsigned long long v;

	if (!val)
		return val;

	/* clz = count-leading-zeros. bitpos is the position of the most significant bit,
	relative to "1" or 1 << base */
	bitpos = WOBBLER_FIXBITS - int32_tclz(val);

	/* Calculate our first estimate.
	We use the identity 2^a * 2^a = 2^(2*a) or:
	sqrt(2^a) = 2^(a/2)
	*/
	if (bitpos > 0u) /* val > 1 */
		x = (1u << WOBBLER_FIXBITS) << (bitpos >> 1u);
	else if (bitpos < 0) /* 0 < val < 1 */
		x = (1u << WOBBLER_FIXBITS) << ((unsigned)(-bitpos) << 1u);
	else /* val == 1 */
		x = (1u << WOBBLER_FIXBITS);

	/* We need to scale val with base due to the division.
	Also val /= 2, hence the subtraction of one*/
	v = (unsigned long long)val << (WOBBLER_FIXBITS - 1u);

	/* The actual iteration */
	x = (x >> 1u) + v / x;
	x = (x >> 1u) + v / x;
	x = (x >> 1u) + v / x;
	x = (x >> 1u) + v / x;
	return x;
}



void UpdateSystem(struct SpringMassSystem_t *P)
{
	for (int i = 0; i < P->MassCount; i++)
	{
		P->Masses[i].Force.X = 0;
		P->Masses[i].Force.Y = F(0.1);
	}

	for (int i = 0; i < P->SpringCount; i++)
	{
		// calc distances
		Mass_t *A = &P->Masses[P->Springs[i].A];
		Mass_t *B = &P->Masses[P->Springs[i].B];

		int32_t dx = B->Pos.X - A->Pos.X;
		int32_t dy = B->Pos.Y - A->Pos.Y;

		uint32_t lengthsquared = imul(dx, dx) + imul(dy, dy);

		int32_t len = fp_sqrt(lengthsquared);


		if (len > 0)
		{
			int32_t extension = P->Springs[i].RestLength - len;
			State St;
			St.m_position = extension;

			int32_t dxnorm = idiv(dx, len);
			int32_t dynorm = idiv(dy, len);
			float dxf = dx / (float)(1 << WOBBLER_FIXBITS);
			float dyf = dy / (float)(1 << WOBBLER_FIXBITS);
			float lenf = len / (float)(1 << WOBBLER_FIXBITS);
			float extf = extension / (float)(1 << WOBBLER_FIXBITS);;
			extf *= 0.01;
			dxf /= lenf;
			dyf /= lenf;
			if (A->fixed == 1) // add double to B
			{
				B->Force.X += (dxf * extf * 1)*(float)(1 << WOBBLER_FIXBITS);
				B->Force.Y += (dyf * extf * 1)*(float)(1 << WOBBLER_FIXBITS);
			}
			else
			{
				if (B->fixed == 1) // subtract double from A
				{
					A->Force.X -= (dxf * extf * 1)*(float)(1 << WOBBLER_FIXBITS);
					A->Force.Y -= (dyf * extf * 1)*(float)(1 << WOBBLER_FIXBITS);
				}
				else
				{
					// distribute evenly.
					B->Force.X += (dxf * extf)*(float)(1 << WOBBLER_FIXBITS);
					B->Force.Y += (dyf * extf)*(float)(1 << WOBBLER_FIXBITS);
					A->Force.X -= (dxf * extf)*(float)(1 << WOBBLER_FIXBITS);
					A->Force.Y -= (dyf * extf)*(float)(1 << WOBBLER_FIXBITS);
				}
			}
		}
	}

	for (int i = 0; i < P->MassCount; i++)
	{
		if (P->Masses[i].fixed == 0)
		{
			P->Masses[i].Pos.X += (P->Masses[i].Pos.X - P->Masses[i].PrevPos.X) + P->Masses[i].Force.X;
			P->Masses[i].Pos.Y += (P->Masses[i].Pos.Y - P->Masses[i].PrevPos.Y) + P->Masses[i].Force.Y;

			P->Masses[i].PrevPos.X = P->Masses[i].Pos.X;
			P->Masses[i].PrevPos.Y = P->Masses[i].Pos.Y;
		}
		else
		{
			P->Masses[i].Speed.X = 0;
			P->Masses[i].Speed.Y = 0;
		}
	}
}

#endif