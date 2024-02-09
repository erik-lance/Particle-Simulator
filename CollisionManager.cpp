#include "CollisionManager.h"

CollisionManager::CollisionManager(int width, int height)
{
	setSimulatorDimensions(width, height);
	setGridDimensions(10, 10);
}

CollisionManager::CollisionManager(int width, int height, int columns, int rows)
{
	setSimulatorDimensions(width, height);
	setGridDimensions(columns, rows);
}

CollisionManager::~CollisionManager()
{
}

/**
 * Sets up the grid dimensions, cell sizes, and initializes the grid
 * @param columns The number of columns in the grid
 * @param rows The number of rows in the grid
 */
void CollisionManager::setGridDimensions(int columns, int rows)
{
	grid_columns = columns;
	grid_rows = rows;

	grid_cell_width = simulator_width / grid_columns;
	grid_cell_height = simulator_height / grid_rows;

	// Initialize the grid
	grid.columns = grid_columns;
	grid.rows = grid_rows;
	grid.cell_width = grid_cell_width;
	grid.cell_height = grid_cell_height;

	// Allocate memory for the grid
	grid.cells = new Cell * [grid_columns];
	for (int i = 0; i < grid_columns; i++)
	{
		grid.cells[i] = new Cell[grid_rows];
	}

	// Initialize the cells
	for (int i = 0; i < grid_columns; i++)
	{
		for (int j = 0; j < grid_rows; j++)
		{
			Position pos{ i * grid_cell_width, j * grid_cell_height };
			grid.cells[i][j].position = pos;
			grid.cells[i][j].maxParticles = 1024;
			grid.cells[i][j].maxWalls = 1024;
			grid.cells[i][j].numParticles = 0;
			grid.cells[i][j].numWalls = 0;
			grid.cells[i][j].particles = new Particle[grid.cells[i][j].maxParticles];
			grid.cells[i][j].walls = new Wall[grid.cells[i][j].maxWalls];
		}
	}
}

/**
 * Gets the grid cell given the particle's position
 * @param x The particle's x position
 * @param y The particle's y position
 * @return A pair of integers representing the grid cell
 */
Cell CollisionManager::getGridCell(int x, int y)
{
	// Calculates the particle position to correspond to the grid
	int cell_x = x / grid_cell_width;
	int cell_y = y / grid_cell_height;

	// Clamp the cell to the grid
	if (cell_x < 0) cell_x = 0;
	if (cell_x >= grid_columns) cell_x = grid_columns - 1;
	if (cell_y < 0) cell_y = 0;
	if (cell_y >= grid_rows) cell_y = grid_rows - 1;

	return Cell{ cell_x, cell_y };
}

/**
 * Updates the particle grid with the particle's position. Called when the particle moves
 * @param id The particle's ID
 * @param cell The particle's grid cell
 * @param x The particle's x position
 * @param y The particle's y position
 */
void CollisionManager::updateParticleGrid(int id, Cell cell, int x, int y)
{
	// Get the grid cell
	Cell new_cell = getGridCell(x, y);

	// Update the grid cell with the particle's ID
}

void CollisionManager::updateLineGrid(int id, int x1, int y1, int x2, int y2)
{
}

/**
 * Checks for collisions within the grid. It searches for
 * cells where there is at least one line and one particle.
 */
void CollisionManager::checkCollisions()
{
	// Iterate through each cell in the grid
	for (int i = 0; i < grid_columns; i++)
	{
		for (int j = 0; j < grid_rows; j++)
		{
			// Get the cell
			Cell cell = grid.cells[i][j];

			// Check if the cell has particles and lines
			if (cell.numParticles > 0 && cell.numWalls > 0)
			{
				// Check for collisions in the cell
				checkParticleLineCollisionsInCell(cell);
			}
		}
	}
}

/**
 * Checks for collisions in a column
 * @param column The column to check for collisions
 */
void CollisionManager::checkCollisionsColumn(int column)
{
	// Iterate through each cell in the column
	for (int i = 0; i < grid_rows; i++)
	{
		// Get the cell
		Cell cell = grid.cells[column][i];

		// Check if the cell has particles and lines
		if (cell.numParticles > 0 && cell.numWalls > 0)
		{
			// Check for collisions in the cell
			checkParticleLineCollisionsInCell(cell);
		}
	}
}

/**
 * Checks for collisions in a row
 * @param row The row to check for collisions
 */
void CollisionManager::checkCollisionsRow(int row)
{
	// Iterate through each cell in the row
	for (int i = 0; i < grid_columns; i++)
	{
		// Get the cell
		Cell cell = grid.cells[i][row];

		// Check if the cell has particles and lines
		if (cell.numParticles > 0 && cell.numWalls > 0)
		{
			// Check for collisions in the cell
			checkParticleLineCollisionsInCell(cell);
		}
	}
}

/**
 * Checks for particle-line collisions in a cell
 * @param cell The cell to check for collisions
 */
void CollisionManager::checkParticleLineCollisionsInCell(Cell cell)
{
	// Check for collisions between particles and lines in the cell
	for (int i = 0; i < cell.numParticles; i++)
	{
		// Get the particle
		Particle p = cell.particles[i];

		// Check for collisions with lines
		for (int j = 0; j < cell.numWalls; j++)
		{
			// Get the line
			Wall l = cell.walls[j];

			// Line
			Line line = l.getLine();

			// Check for collisions
			bool collided = p.handleLineCollision(line.x1, line.y1, line.x2, line.y2);

			// If there is a collision, stop checking for collisions for this particle
			if (collided) break;
		}
	}
}
