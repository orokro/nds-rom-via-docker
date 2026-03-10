#include <nds.h>
#include <stdio.h>

#define TEX_SIZE	64
#define BUTTON_X1	16
#define BUTTON_Y1	120
#define BUTTON_X2	120
#define BUTTON_Y2	168

static u16 texture[TEX_SIZE * TEX_SIZE];
static int textureID = 0;

static int rotationDir = 1;
static int angleY = 0;

static PrintConsole bottomScreen;

static void makeCheckerTexture(void)
{
	for (int y = 0; y < TEX_SIZE; y++)
	{
		for (int x = 0; x < TEX_SIZE; x++)
		{
			int checker = ((x >> 3) ^ (y >> 3)) & 1;

			u16 color;
			if (checker)
			{
				color = RGB15(8, 28, 28) | BIT(15);
			}
			else
			{
				color = RGB15(2, 6, 14) | BIT(15);
			}

			texture[y * TEX_SIZE + x] = color;
		}
	}
}

static void init3D(void)
{
	videoSetMode(MODE_0_3D);

	vramSetBankA(VRAM_A_TEXTURE);
	vramSetBankB(VRAM_B_TEXTURE);

	glInit();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ANTIALIAS);
	glEnable(GL_BLEND);

	glClearColor(0, 0, 0, 31);
	glClearPolyID(63);
	glClearDepth(GL_MAX_DEPTH);

	glViewport(0, 0, 255, 191);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 40.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	makeCheckerTexture();

	glGenTextures(1, &textureID);
	glBindTexture(0, textureID);
	glTexImage2D(
		0,
		0,
		GL_RGBA,
		TEX_SIZE,
		TEX_SIZE,
		0,
		TEXGEN_TEXCOORD,
		texture
	);

	glTexParameter(0, GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T);
}

static void initBottomUI(void)
{
	videoSetModeSub(MODE_0_2D);
	vramSetBankC(VRAM_C_SUB_BG);

	consoleInit(
		&bottomScreen,
		0,
		BgType_Text4bpp,
		BgSize_T_256x256,
		31,
		0,
		false,
		true
	);

	consoleSelect(&bottomScreen);
	iprintf("\x1b[2J");
}

static void drawBottomUI(void)
{
	consoleSelect(&bottomScreen);
	iprintf("\x1b[2J");

	iprintf("Bottom Screen UI\n");
	iprintf("----------------\n\n");
	iprintf("Touch the box below\n");
	iprintf("to flip cube rotation.\n\n");

	iprintf("Rotation: %s\n\n", rotationDir > 0 ? "CLOCKWISE" : "COUNTER-CLOCKWISE");

	iprintf("+-------------+\n");
	iprintf("| FLIP DIR    |\n");
	iprintf("| touch here  |\n");
	iprintf("+-------------+\n");

	iprintf("\nStylus X/Y shown below.\n");
}

static void drawQuad(
	v16 x1, v16 y1, v16 z1,
	v16 x2, v16 y2, v16 z2,
	v16 x3, v16 y3, v16 z3,
	v16 x4, v16 y4, v16 z4)
{
	glBegin(GL_QUADS);

	glTexCoord2t16(0, 0);
	glVertex3v16(x1, y1, z1);

	glTexCoord2t16(inttot16(TEX_SIZE), 0);
	glVertex3v16(x2, y2, z2);

	glTexCoord2t16(inttot16(TEX_SIZE), inttot16(TEX_SIZE));
	glVertex3v16(x3, y3, z3);

	glTexCoord2t16(0, inttot16(TEX_SIZE));
	glVertex3v16(x4, y4, z4);

	glEnd();
}

static void drawCube(void)
{
	const v16 s = floattov16(1.0f);

	glBindTexture(0, textureID);

	glColor3b(255, 255, 255);
	drawQuad(-s, -s, s,  s, -s, s,  s, s, s,  -s, s, s);

	glColor3b(220, 220, 220);
	drawQuad( s, -s, -s, -s, -s, -s, -s, s, -s,  s, s, -s);

	glColor3b(200, 200, 255);
	drawQuad(-s, -s, -s, -s, -s, s, -s, s, s, -s, s, -s);

	glColor3b(255, 200, 200);
	drawQuad( s, -s, s,  s, -s, -s,  s, s, -s,  s, s, s);

	glColor3b(200, 255, 200);
	drawQuad(-s, s, s,  s, s, s,  s, s, -s, -s, s, -s);

	glColor3b(255, 255, 200);
	drawQuad(-s, -s, -s,  s, -s, -s,  s, -s, s, -s, -s, s);
}

static void updateTouchUI(void)
{
	touchPosition touch;
	touchRead(&touch);

	consoleSelect(&bottomScreen);
	iprintf("\x1b[14;0H");
	iprintf("Stylus: %3d, %3d   ", touch.px, touch.py);

	if (keysDown() & KEY_TOUCH)
	{
		if (touch.px >= BUTTON_X1 && touch.px <= BUTTON_X2 &&
			touch.py >= BUTTON_Y1 && touch.py <= BUTTON_Y2)
		{
			rotationDir = -rotationDir;
			drawBottomUI();
		}
	}
}

int main(void)
{
	lcdMainOnTop();

	init3D();
	initBottomUI();
	drawBottomUI();

	while (pmMainLoop())
	{
		scanKeys();
		updateTouchUI();

		angleY += rotationDir * 2;

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef32(0, 0, floattof32(-3.5f));
		glRotateXi(angleY >> 1);
		glRotateYi(angleY);

		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK);

		drawCube();

		glFlush(0);
		swiWaitForVBlank();
	}

	return 0;
}