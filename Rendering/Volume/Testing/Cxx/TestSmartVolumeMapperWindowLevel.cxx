/*=========================================================================

  Program:   Visualization Toolkit
  Module:    TestSmartVolumeMapperWindowLevel.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// This test covers the smart volume mapper and composite method with a
// customed window/level values (brightness/contrast).
// This test volume renders a synthetic dataset with unsigned char values,
// with the composite method.

#include "vtkColorTransferFunction.h"
#include "vtkDataArray.h"
#include "vtkImageData.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkMetaImageReader.h"
#include "vtkNew.h"
#include "vtkPiecewiseFunction.h"
#include "vtkPointData.h"
#include "vtkRegressionTestImage.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartVolumeMapper.h"
#include "vtkTestUtilities.h"
#include "vtkVolumeProperty.h"

static const char * TestSmartVolumeMapperWindowLevelLog =
"# StreamVersion 1\n"
"EnterEvent 398 137 0 0 0 0 0\n"
"MouseMoveEvent 398 137 0 0 0 0 0\n"
"MouseMoveEvent 390 145 0 0 0 0 0\n"
"MouseMoveEvent 380 154 0 0 0 0 0\n"
"MouseMoveEvent 371 161 0 0 0 0 0\n"
"MouseMoveEvent 362 165 0 0 0 0 0\n"
"MouseMoveEvent 356 170 0 0 0 0 0\n"
"MouseMoveEvent 349 175 0 0 0 0 0\n"
"MouseMoveEvent 342 180 0 0 0 0 0\n"
"MouseMoveEvent 338 184 0 0 0 0 0\n"
"MouseMoveEvent 333 186 0 0 0 0 0\n"
"MouseMoveEvent 329 192 0 0 0 0 0\n"
"MouseMoveEvent 324 199 0 0 0 0 0\n"
"MouseMoveEvent 320 205 0 0 0 0 0\n"
"MouseMoveEvent 315 211 0 0 0 0 0\n"
"MouseMoveEvent 311 217 0 0 0 0 0\n"
"MouseMoveEvent 307 224 0 0 0 0 0\n"
"MouseMoveEvent 302 229 0 0 0 0 0\n"
"MouseMoveEvent 298 234 0 0 0 0 0\n"
"MouseMoveEvent 294 238 0 0 0 0 0\n"
"MouseMoveEvent 290 242 0 0 0 0 0\n"
"MouseMoveEvent 286 246 0 0 0 0 0\n"
"MouseMoveEvent 282 250 0 0 0 0 0\n"
"MouseMoveEvent 278 253 0 0 0 0 0\n"
"MouseMoveEvent 277 257 0 0 0 0 0\n"
"MouseMoveEvent 274 261 0 0 0 0 0\n"
"MouseMoveEvent 273 265 0 0 0 0 0\n"
"MouseMoveEvent 273 266 0 0 0 0 0\n"
"MouseMoveEvent 274 267 0 0 0 0 0\n"
"MouseMoveEvent 275 267 0 0 0 0 0\n"
"MouseMoveEvent 276 267 0 0 0 0 0\n"
"MouseMoveEvent 277 266 0 0 0 0 0\n"
"MouseMoveEvent 278 265 0 0 0 0 0\n"
"MouseMoveEvent 279 264 0 0 0 0 0\n"
"MouseMoveEvent 280 263 0 0 0 0 0\n"
"MouseMoveEvent 280 262 0 0 0 0 0\n"
"MouseMoveEvent 281 260 0 0 0 0 0\n"
"MouseMoveEvent 282 259 0 0 0 0 0\n"
"MouseMoveEvent 282 258 0 0 0 0 0\n"
"MouseMoveEvent 282 257 0 0 0 0 0\n"
"MouseMoveEvent 283 256 0 0 0 0 0\n"
"MouseMoveEvent 284 255 0 0 0 0 0\n"
"MouseMoveEvent 284 254 0 0 0 0 0\n"
"MouseMoveEvent 284 253 0 0 0 0 0\n"
"MouseMoveEvent 285 252 0 0 0 0 0\n"
"RightButtonPressEvent 285 252 0 0 0 0 0\n"
"StartInteractionEvent 285 252 0 0 0 0 0\n"
"MouseMoveEvent 285 251 0 0 0 0 0\n"
"RenderEvent 285 251 0 0 0 0 0\n"
"MouseMoveEvent 285 249 0 0 0 0 0\n"
"RenderEvent 285 249 0 0 0 0 0\n"
"MouseMoveEvent 285 248 0 0 0 0 0\n"
"RenderEvent 285 248 0 0 0 0 0\n"
"MouseMoveEvent 285 247 0 0 0 0 0\n"
"RenderEvent 285 247 0 0 0 0 0\n"
"MouseMoveEvent 285 243 0 0 0 0 0\n"
"RenderEvent 285 243 0 0 0 0 0\n"
"MouseMoveEvent 285 240 0 0 0 0 0\n"
"RenderEvent 285 240 0 0 0 0 0\n"
"MouseMoveEvent 285 237 0 0 0 0 0\n"
"RenderEvent 285 237 0 0 0 0 0\n"
"MouseMoveEvent 285 236 0 0 0 0 0\n"
"RenderEvent 285 236 0 0 0 0 0\n"
"MouseMoveEvent 285 234 0 0 0 0 0\n"
"RenderEvent 285 234 0 0 0 0 0\n"
"MouseMoveEvent 285 230 0 0 0 0 0\n"
"RenderEvent 285 230 0 0 0 0 0\n"
"MouseMoveEvent 286 227 0 0 0 0 0\n"
"RenderEvent 286 227 0 0 0 0 0\n"
"MouseMoveEvent 286 222 0 0 0 0 0\n"
"RenderEvent 286 222 0 0 0 0 0\n"
"MouseMoveEvent 287 217 0 0 0 0 0\n"
"RenderEvent 287 217 0 0 0 0 0\n"
"MouseMoveEvent 288 213 0 0 0 0 0\n"
"RenderEvent 288 213 0 0 0 0 0\n"
"MouseMoveEvent 288 208 0 0 0 0 0\n"
"RenderEvent 288 208 0 0 0 0 0\n"
"MouseMoveEvent 289 205 0 0 0 0 0\n"
"RenderEvent 289 205 0 0 0 0 0\n"
"MouseMoveEvent 289 202 0 0 0 0 0\n"
"RenderEvent 289 202 0 0 0 0 0\n"
"MouseMoveEvent 289 200 0 0 0 0 0\n"
"RenderEvent 289 200 0 0 0 0 0\n"
"MouseMoveEvent 290 198 0 0 0 0 0\n"
"RenderEvent 290 198 0 0 0 0 0\n"
"MouseMoveEvent 290 196 0 0 0 0 0\n"
"RenderEvent 290 196 0 0 0 0 0\n"
"MouseMoveEvent 290 195 0 0 0 0 0\n"
"RenderEvent 290 195 0 0 0 0 0\n"
"MouseMoveEvent 290 192 0 0 0 0 0\n"
"RenderEvent 290 192 0 0 0 0 0\n"
"MouseMoveEvent 291 188 0 0 0 0 0\n"
"RenderEvent 291 188 0 0 0 0 0\n"
"MouseMoveEvent 291 186 0 0 0 0 0\n"
"RenderEvent 291 186 0 0 0 0 0\n"
"MouseMoveEvent 291 184 0 0 0 0 0\n"
"RenderEvent 291 184 0 0 0 0 0\n"
"MouseMoveEvent 291 183 0 0 0 0 0\n"
"RenderEvent 291 183 0 0 0 0 0\n"
"MouseMoveEvent 291 182 0 0 0 0 0\n"
"RenderEvent 291 182 0 0 0 0 0\n"
"MouseMoveEvent 291 181 0 0 0 0 0\n"
"RenderEvent 291 181 0 0 0 0 0\n"
"MouseMoveEvent 291 180 0 0 0 0 0\n"
"RenderEvent 291 180 0 0 0 0 0\n"
"MouseMoveEvent 291 179 0 0 0 0 0\n"
"RenderEvent 291 179 0 0 0 0 0\n"
"MouseMoveEvent 291 178 0 0 0 0 0\n"
"RenderEvent 291 178 0 0 0 0 0\n"
"MouseMoveEvent 292 177 0 0 0 0 0\n"
"RenderEvent 292 177 0 0 0 0 0\n"
"MouseMoveEvent 292 176 0 0 0 0 0\n"
"RenderEvent 292 176 0 0 0 0 0\n"
"MouseMoveEvent 293 173 0 0 0 0 0\n"
"RenderEvent 293 173 0 0 0 0 0\n"
"MouseMoveEvent 293 174 0 0 0 0 0\n"
"RenderEvent 293 174 0 0 0 0 0\n"
"MouseMoveEvent 293 175 0 0 0 0 0\n"
"RenderEvent 293 175 0 0 0 0 0\n"
"MouseMoveEvent 293 176 0 0 0 0 0\n"
"RenderEvent 293 176 0 0 0 0 0\n"
"MouseMoveEvent 294 177 0 0 0 0 0\n"
"RenderEvent 294 177 0 0 0 0 0\n"
"MouseMoveEvent 294 179 0 0 0 0 0\n"
"RenderEvent 294 179 0 0 0 0 0\n"
"MouseMoveEvent 294 180 0 0 0 0 0\n"
"RenderEvent 294 180 0 0 0 0 0\n"
"MouseMoveEvent 294 182 0 0 0 0 0\n"
"RenderEvent 294 182 0 0 0 0 0\n"
"MouseMoveEvent 295 186 0 0 0 0 0\n"
"RenderEvent 295 186 0 0 0 0 0\n"
"MouseMoveEvent 296 190 0 0 0 0 0\n"
"RenderEvent 296 190 0 0 0 0 0\n"
"MouseMoveEvent 296 192 0 0 0 0 0\n"
"RenderEvent 296 192 0 0 0 0 0\n"
"MouseMoveEvent 296 195 0 0 0 0 0\n"
"RenderEvent 296 195 0 0 0 0 0\n"
"MouseMoveEvent 297 197 0 0 0 0 0\n"
"RenderEvent 297 197 0 0 0 0 0\n"
"MouseMoveEvent 297 198 0 0 0 0 0\n"
"RenderEvent 297 198 0 0 0 0 0\n"
"MouseMoveEvent 297 201 0 0 0 0 0\n"
"RenderEvent 297 201 0 0 0 0 0\n"
"MouseMoveEvent 298 204 0 0 0 0 0\n"
"RenderEvent 298 204 0 0 0 0 0\n"
"MouseMoveEvent 298 207 0 0 0 0 0\n"
"RenderEvent 298 207 0 0 0 0 0\n"
"MouseMoveEvent 299 212 0 0 0 0 0\n"
"RenderEvent 299 212 0 0 0 0 0\n"
"MouseMoveEvent 299 218 0 0 0 0 0\n"
"RenderEvent 299 218 0 0 0 0 0\n"
"MouseMoveEvent 299 226 0 0 0 0 0\n"
"RenderEvent 299 226 0 0 0 0 0\n"
"MouseMoveEvent 299 237 0 0 0 0 0\n"
"RenderEvent 299 237 0 0 0 0 0\n"
"MouseMoveEvent 299 242 0 0 0 0 0\n"
"RenderEvent 299 242 0 0 0 0 0\n"
"MouseMoveEvent 299 247 0 0 0 0 0\n"
"RenderEvent 299 247 0 0 0 0 0\n"
"MouseMoveEvent 299 253 0 0 0 0 0\n"
"RenderEvent 299 253 0 0 0 0 0\n"
"MouseMoveEvent 299 258 0 0 0 0 0\n"
"RenderEvent 299 258 0 0 0 0 0\n"
"MouseMoveEvent 299 263 0 0 0 0 0\n"
"RenderEvent 299 263 0 0 0 0 0\n"
"MouseMoveEvent 299 267 0 0 0 0 0\n"
"RenderEvent 299 267 0 0 0 0 0\n"
"MouseMoveEvent 299 271 0 0 0 0 0\n"
"RenderEvent 299 271 0 0 0 0 0\n"
"MouseMoveEvent 299 276 0 0 0 0 0\n"
"RenderEvent 299 276 0 0 0 0 0\n"
"MouseMoveEvent 299 281 0 0 0 0 0\n"
"RenderEvent 299 281 0 0 0 0 0\n"
"MouseMoveEvent 299 285 0 0 0 0 0\n"
"RenderEvent 299 285 0 0 0 0 0\n"
"MouseMoveEvent 299 288 0 0 0 0 0\n"
"RenderEvent 299 288 0 0 0 0 0\n"
"MouseMoveEvent 299 292 0 0 0 0 0\n"
"RenderEvent 299 292 0 0 0 0 0\n"
"MouseMoveEvent 299 296 0 0 0 0 0\n"
"RenderEvent 299 296 0 0 0 0 0\n"
"MouseMoveEvent 299 301 0 0 0 0 0\n"
"RenderEvent 299 301 0 0 0 0 0\n"
"MouseMoveEvent 299 308 0 0 0 0 0\n"
"RenderEvent 299 308 0 0 0 0 0\n"
"MouseMoveEvent 299 314 0 0 0 0 0\n"
"RenderEvent 299 314 0 0 0 0 0\n"
"MouseMoveEvent 299 321 0 0 0 0 0\n"
"RenderEvent 299 321 0 0 0 0 0\n"
"MouseMoveEvent 299 334 0 0 0 0 0\n"
"RenderEvent 299 334 0 0 0 0 0\n"
"MouseMoveEvent 299 337 0 0 0 0 0\n"
"RenderEvent 299 337 0 0 0 0 0\n"
"MouseMoveEvent 299 347 0 0 0 0 0\n"
"RenderEvent 299 347 0 0 0 0 0\n"
"MouseMoveEvent 299 358 0 0 0 0 0\n"
"RenderEvent 299 358 0 0 0 0 0\n"
"MouseMoveEvent 299 370 0 0 0 0 0\n"
"RenderEvent 299 370 0 0 0 0 0\n"
"MouseMoveEvent 299 383 0 0 0 0 0\n"
"RenderEvent 299 383 0 0 0 0 0\n"
"MouseMoveEvent 299 390 0 0 0 0 0\n"
"RenderEvent 299 390 0 0 0 0 0\n"
"MouseMoveEvent 299 394 0 0 0 0 0\n"
"RenderEvent 299 394 0 0 0 0 0\n"
"MouseMoveEvent 299 397 0 0 0 0 0\n"
"RenderEvent 299 397 0 0 0 0 0\n"
"MouseMoveEvent 299 401 0 0 0 0 0\n"
"RenderEvent 299 401 0 0 0 0 0\n"
"LeaveEvent 299 401 0 0 0 0 0\n"
"MouseMoveEvent 299 405 0 0 0 0 0\n"
"RenderEvent 299 405 0 0 0 0 0\n"
"MouseMoveEvent 299 410 0 0 0 0 0\n"
"RenderEvent 299 410 0 0 0 0 0\n"
"MouseMoveEvent 299 415 0 0 0 0 0\n"
"RenderEvent 299 415 0 0 0 0 0\n"
"MouseMoveEvent 300 419 0 0 0 0 0\n"
"RenderEvent 300 419 0 0 0 0 0\n"
"MouseMoveEvent 300 422 0 0 0 0 0\n"
"RenderEvent 300 422 0 0 0 0 0\n"
"MouseMoveEvent 301 426 0 0 0 0 0\n"
"RenderEvent 301 426 0 0 0 0 0\n"
"MouseMoveEvent 303 431 0 0 0 0 0\n"
"RenderEvent 303 431 0 0 0 0 0\n"
"MouseMoveEvent 304 433 0 0 0 0 0\n"
"RenderEvent 304 433 0 0 0 0 0\n"
"MouseMoveEvent 304 433 0 0 0 0 0\n"
"RenderEvent 304 433 0 0 0 0 0\n"
"RightButtonReleaseEvent 304 433 0 0 0 0 0\n"
"EndInteractionEvent 304 433 0 0 0 0 0\n"
"RenderEvent 304 433 0 0 0 0 0\n"
"LeaveEvent 304 433 0 0 0 0 0\n"
"EnterEvent 300 399 0 0 0 0 0\n"
"MouseMoveEvent 300 399 0 0 0 0 0\n"
"MouseMoveEvent 300 395 0 0 0 0 0\n"
"MouseMoveEvent 300 389 0 0 0 0 0\n"
"MouseMoveEvent 300 383 0 0 0 0 0\n"
"MouseMoveEvent 300 376 0 0 0 0 0\n"
"MouseMoveEvent 300 369 0 0 0 0 0\n"
"MouseMoveEvent 300 362 0 0 0 0 0\n"
"MouseMoveEvent 298 355 0 0 0 0 0\n"
"MouseMoveEvent 297 347 0 0 0 0 0\n"
"MouseMoveEvent 297 338 0 0 0 0 0\n"
"MouseMoveEvent 294 328 0 0 0 0 0\n"
"MouseMoveEvent 294 319 0 0 0 0 0\n"
"MouseMoveEvent 292 309 0 0 0 0 0\n"
"MouseMoveEvent 290 299 0 0 0 0 0\n"
"MouseMoveEvent 289 289 0 0 0 0 0\n"
"MouseMoveEvent 289 279 0 0 0 0 0\n"
"MouseMoveEvent 289 271 0 0 0 0 0\n"
"MouseMoveEvent 289 262 0 0 0 0 0\n"
"MouseMoveEvent 287 254 0 0 0 0 0\n"
"MouseMoveEvent 285 246 0 0 0 0 0\n"
"MouseMoveEvent 285 238 0 0 0 0 0\n"
"MouseMoveEvent 283 229 0 0 0 0 0\n"
"MouseMoveEvent 281 221 0 0 0 0 0\n"
"MouseMoveEvent 281 214 0 0 0 0 0\n"
"MouseMoveEvent 281 208 0 0 0 0 0\n"
"MouseMoveEvent 281 202 0 0 0 0 0\n"
"MouseMoveEvent 281 198 0 0 0 0 0\n"
"MouseMoveEvent 281 197 0 0 0 0 0\n"
"MouseMoveEvent 281 196 0 0 0 0 0\n"
"MouseMoveEvent 281 195 0 0 0 0 0\n"
"MouseMoveEvent 281 194 0 0 0 0 0\n"
"MouseMoveEvent 281 195 0 0 0 0 0\n"
"MouseMoveEvent 281 196 0 0 0 0 0\n"
"MouseMoveEvent 281 197 0 0 0 0 0\n"
"MouseMoveEvent 281 198 0 0 0 0 0\n"
"MouseMoveEvent 281 199 0 0 0 0 0\n"
"LeftButtonPressEvent 281 199 0 0 0 0 0\n"
"StartInteractionEvent 281 199 0 0 0 0 0\n"
"MouseMoveEvent 281 197 0 0 0 0 0\n"
"RenderEvent 281 197 0 0 0 0 0\n"
"MouseMoveEvent 281 195 0 0 0 0 0\n"
"RenderEvent 281 195 0 0 0 0 0\n"
"MouseMoveEvent 281 193 0 0 0 0 0\n"
"RenderEvent 281 193 0 0 0 0 0\n"
"MouseMoveEvent 280 188 0 0 0 0 0\n"
"RenderEvent 280 188 0 0 0 0 0\n"
"MouseMoveEvent 279 183 0 0 0 0 0\n"
"RenderEvent 279 183 0 0 0 0 0\n"
"MouseMoveEvent 279 178 0 0 0 0 0\n"
"RenderEvent 279 178 0 0 0 0 0\n"
"MouseMoveEvent 279 173 0 0 0 0 0\n"
"RenderEvent 279 173 0 0 0 0 0\n"
"MouseMoveEvent 279 168 0 0 0 0 0\n"
"RenderEvent 279 168 0 0 0 0 0\n"
"MouseMoveEvent 279 163 0 0 0 0 0\n"
"RenderEvent 279 163 0 0 0 0 0\n"
"MouseMoveEvent 279 159 0 0 0 0 0\n"
"RenderEvent 279 159 0 0 0 0 0\n"
"MouseMoveEvent 279 154 0 0 0 0 0\n"
"RenderEvent 279 154 0 0 0 0 0\n"
"MouseMoveEvent 279 149 0 0 0 0 0\n"
"RenderEvent 279 149 0 0 0 0 0\n"
"MouseMoveEvent 279 143 0 0 0 0 0\n"
"RenderEvent 279 143 0 0 0 0 0\n"
"MouseMoveEvent 279 137 0 0 0 0 0\n"
"RenderEvent 279 137 0 0 0 0 0\n"
"MouseMoveEvent 279 129 0 0 0 0 0\n"
"RenderEvent 279 129 0 0 0 0 0\n"
"MouseMoveEvent 279 122 0 0 0 0 0\n"
"RenderEvent 279 122 0 0 0 0 0\n"
"MouseMoveEvent 279 116 0 0 0 0 0\n"
"RenderEvent 279 116 0 0 0 0 0\n"
"MouseMoveEvent 279 109 0 0 0 0 0\n"
"RenderEvent 279 109 0 0 0 0 0\n"
"MouseMoveEvent 279 102 0 0 0 0 0\n"
"RenderEvent 279 102 0 0 0 0 0\n"
"MouseMoveEvent 279 98 0 0 0 0 0\n"
"RenderEvent 279 98 0 0 0 0 0\n"
"MouseMoveEvent 279 95 0 0 0 0 0\n"
"RenderEvent 279 95 0 0 0 0 0\n"
"MouseMoveEvent 279 94 0 0 0 0 0\n"
"RenderEvent 279 94 0 0 0 0 0\n"
"MouseMoveEvent 279 92 0 0 0 0 0\n"
"RenderEvent 279 92 0 0 0 0 0\n"
"MouseMoveEvent 280 90 0 0 0 0 0\n"
"RenderEvent 280 90 0 0 0 0 0\n"
"MouseMoveEvent 280 89 0 0 0 0 0\n"
"RenderEvent 280 89 0 0 0 0 0\n"
"MouseMoveEvent 280 88 0 0 0 0 0\n"
"RenderEvent 280 88 0 0 0 0 0\n"
"MouseMoveEvent 280 87 0 0 0 0 0\n"
"RenderEvent 280 87 0 0 0 0 0\n"
"MouseMoveEvent 280 86 0 0 0 0 0\n"
"RenderEvent 280 86 0 0 0 0 0\n"
"MouseMoveEvent 280 85 0 0 0 0 0\n"
"RenderEvent 280 85 0 0 0 0 0\n"
"MouseMoveEvent 280 84 0 0 0 0 0\n"
"RenderEvent 280 84 0 0 0 0 0\n"
"MouseMoveEvent 280 81 0 0 0 0 0\n"
"RenderEvent 280 81 0 0 0 0 0\n"
"MouseMoveEvent 280 79 0 0 0 0 0\n"
"RenderEvent 280 79 0 0 0 0 0\n"
"MouseMoveEvent 280 77 0 0 0 0 0\n"
"RenderEvent 280 77 0 0 0 0 0\n"
"MouseMoveEvent 280 75 0 0 0 0 0\n"
"RenderEvent 280 75 0 0 0 0 0\n"
"MouseMoveEvent 280 74 0 0 0 0 0\n"
"RenderEvent 280 74 0 0 0 0 0\n"
"MouseMoveEvent 281 73 0 0 0 0 0\n"
"RenderEvent 281 73 0 0 0 0 0\n"
"MouseMoveEvent 281 72 0 0 0 0 0\n"
"RenderEvent 281 72 0 0 0 0 0\n"
"MouseMoveEvent 282 71 0 0 0 0 0\n"
"RenderEvent 282 71 0 0 0 0 0\n"
"MouseMoveEvent 282 72 0 0 0 0 0\n"
"RenderEvent 282 72 0 0 0 0 0\n"
"MouseMoveEvent 283 74 0 0 0 0 0\n"
"RenderEvent 283 74 0 0 0 0 0\n"
"MouseMoveEvent 283 75 0 0 0 0 0\n"
"RenderEvent 283 75 0 0 0 0 0\n"
"MouseMoveEvent 284 77 0 0 0 0 0\n"
"RenderEvent 284 77 0 0 0 0 0\n"
"MouseMoveEvent 285 81 0 0 0 0 0\n"
"RenderEvent 285 81 0 0 0 0 0\n"
"MouseMoveEvent 285 83 0 0 0 0 0\n"
"RenderEvent 285 83 0 0 0 0 0\n"
"MouseMoveEvent 285 87 0 0 0 0 0\n"
"RenderEvent 285 87 0 0 0 0 0\n"
"MouseMoveEvent 286 92 0 0 0 0 0\n"
"RenderEvent 286 92 0 0 0 0 0\n"
"MouseMoveEvent 286 99 0 0 0 0 0\n"
"RenderEvent 286 99 0 0 0 0 0\n"
"MouseMoveEvent 286 108 0 0 0 0 0\n"
"RenderEvent 286 108 0 0 0 0 0\n"
"MouseMoveEvent 286 116 0 0 0 0 0\n"
"RenderEvent 286 116 0 0 0 0 0\n"
"MouseMoveEvent 286 126 0 0 0 0 0\n"
"RenderEvent 286 126 0 0 0 0 0\n"
"MouseMoveEvent 286 134 0 0 0 0 0\n"
"RenderEvent 286 134 0 0 0 0 0\n"
"MouseMoveEvent 286 145 0 0 0 0 0\n"
"RenderEvent 286 145 0 0 0 0 0\n"
"MouseMoveEvent 286 154 0 0 0 0 0\n"
"RenderEvent 286 154 0 0 0 0 0\n"
"MouseMoveEvent 286 162 0 0 0 0 0\n"
"RenderEvent 286 162 0 0 0 0 0\n"
"MouseMoveEvent 286 166 0 0 0 0 0\n"
"RenderEvent 286 166 0 0 0 0 0\n"
"MouseMoveEvent 286 171 0 0 0 0 0\n"
"RenderEvent 286 171 0 0 0 0 0\n"
"MouseMoveEvent 286 176 0 0 0 0 0\n"
"RenderEvent 286 176 0 0 0 0 0\n"
"MouseMoveEvent 286 180 0 0 0 0 0\n"
"RenderEvent 286 180 0 0 0 0 0\n"
"MouseMoveEvent 286 185 0 0 0 0 0\n"
"RenderEvent 286 185 0 0 0 0 0\n"
"MouseMoveEvent 286 189 0 0 0 0 0\n"
"RenderEvent 286 189 0 0 0 0 0\n"
"MouseMoveEvent 286 191 0 0 0 0 0\n"
"RenderEvent 286 191 0 0 0 0 0\n"
"MouseMoveEvent 286 197 0 0 0 0 0\n"
"RenderEvent 286 197 0 0 0 0 0\n"
"MouseMoveEvent 286 201 0 0 0 0 0\n"
"RenderEvent 286 201 0 0 0 0 0\n"
"MouseMoveEvent 286 205 0 0 0 0 0\n"
"RenderEvent 286 205 0 0 0 0 0\n"
"MouseMoveEvent 286 209 0 0 0 0 0\n"
"RenderEvent 286 209 0 0 0 0 0\n"
"MouseMoveEvent 286 212 0 0 0 0 0\n"
"RenderEvent 286 212 0 0 0 0 0\n"
"MouseMoveEvent 286 215 0 0 0 0 0\n"
"RenderEvent 286 215 0 0 0 0 0\n"
"MouseMoveEvent 286 218 0 0 0 0 0\n"
"RenderEvent 286 218 0 0 0 0 0\n"
"MouseMoveEvent 286 221 0 0 0 0 0\n"
"RenderEvent 286 221 0 0 0 0 0\n"
"MouseMoveEvent 286 225 0 0 0 0 0\n"
"RenderEvent 286 225 0 0 0 0 0\n"
"MouseMoveEvent 286 229 0 0 0 0 0\n"
"RenderEvent 286 229 0 0 0 0 0\n"
"MouseMoveEvent 286 233 0 0 0 0 0\n"
"RenderEvent 286 233 0 0 0 0 0\n"
"MouseMoveEvent 286 239 0 0 0 0 0\n"
"RenderEvent 286 239 0 0 0 0 0\n"
"MouseMoveEvent 286 245 0 0 0 0 0\n"
"RenderEvent 286 245 0 0 0 0 0\n"
"MouseMoveEvent 286 252 0 0 0 0 0\n"
"RenderEvent 286 252 0 0 0 0 0\n"
"MouseMoveEvent 287 255 0 0 0 0 0\n"
"RenderEvent 287 255 0 0 0 0 0\n"
"MouseMoveEvent 287 256 0 0 0 0 0\n"
"RenderEvent 287 256 0 0 0 0 0\n"
"MouseMoveEvent 287 257 0 0 0 0 0\n"
"RenderEvent 287 257 0 0 0 0 0\n"
"MouseMoveEvent 287 259 0 0 0 0 0\n"
"RenderEvent 287 259 0 0 0 0 0\n"
"MouseMoveEvent 288 261 0 0 0 0 0\n"
"RenderEvent 288 261 0 0 0 0 0\n"
"MouseMoveEvent 288 263 0 0 0 0 0\n"
"RenderEvent 288 263 0 0 0 0 0\n"
"MouseMoveEvent 288 265 0 0 0 0 0\n"
"RenderEvent 288 265 0 0 0 0 0\n"
"MouseMoveEvent 288 266 0 0 0 0 0\n"
"RenderEvent 288 266 0 0 0 0 0\n"
"MouseMoveEvent 289 268 0 0 0 0 0\n"
"RenderEvent 289 268 0 0 0 0 0\n"
"MouseMoveEvent 289 269 0 0 0 0 0\n"
"RenderEvent 289 269 0 0 0 0 0\n"
"MouseMoveEvent 289 272 0 0 0 0 0\n"
"RenderEvent 289 272 0 0 0 0 0\n"
"MouseMoveEvent 290 274 0 0 0 0 0\n"
"RenderEvent 290 274 0 0 0 0 0\n"
"MouseMoveEvent 290 275 0 0 0 0 0\n"
"RenderEvent 290 275 0 0 0 0 0\n"
"MouseMoveEvent 290 276 0 0 0 0 0\n"
"RenderEvent 290 276 0 0 0 0 0\n"
"MouseMoveEvent 291 277 0 0 0 0 0\n"
"RenderEvent 291 277 0 0 0 0 0\n"
"LeftButtonReleaseEvent 291 277 0 0 0 0 0\n"
"EndInteractionEvent 291 277 0 0 0 0 0\n"
"RenderEvent 291 277 0 0 0 0 0\n"
"MouseMoveEvent 292 278 0 0 0 0 0\n"
"MouseMoveEvent 293 277 0 0 0 0 0\n"
"MouseMoveEvent 293 276 0 0 0 0 0\n"
"MouseMoveEvent 293 275 0 0 0 0 0\n"
"MouseMoveEvent 293 274 0 0 0 0 0\n"
"MouseMoveEvent 293 273 0 0 0 0 0\n"
"MouseMoveEvent 293 271 0 0 0 0 0\n"
"MouseMoveEvent 293 270 0 0 0 0 0\n"
"MouseMoveEvent 293 269 0 0 0 0 0\n"
"MouseMoveEvent 294 268 0 0 0 0 0\n"
"MouseMoveEvent 294 266 0 0 0 0 0\n"
"MouseMoveEvent 294 264 0 0 0 0 0\n"
"MouseMoveEvent 295 263 0 0 0 0 0\n"
"MouseMoveEvent 295 261 0 0 0 0 0\n"
"MouseMoveEvent 295 259 0 0 0 0 0\n"
"MouseMoveEvent 296 256 0 0 0 0 0\n"
"MouseMoveEvent 296 254 0 0 0 0 0\n"
"MouseMoveEvent 297 253 0 0 0 0 0\n"
"MouseMoveEvent 298 250 0 0 0 0 0\n"
"MouseMoveEvent 299 248 0 0 0 0 0\n"
"MouseMoveEvent 301 245 0 0 0 0 0\n"
"MouseMoveEvent 302 242 0 0 0 0 0\n"
"MouseMoveEvent 302 241 0 0 0 0 0\n"
"MouseMoveEvent 303 239 0 0 0 0 0\n"
"MouseMoveEvent 303 238 0 0 0 0 0\n"
"MouseMoveEvent 303 235 0 0 0 0 0\n"
"MouseMoveEvent 304 234 0 0 0 0 0\n"
"MouseMoveEvent 304 232 0 0 0 0 0\n"
"MouseMoveEvent 304 229 0 0 0 0 0\n"
"MouseMoveEvent 304 227 0 0 0 0 0\n"
"MouseMoveEvent 304 225 0 0 0 0 0\n"
"MouseMoveEvent 304 223 0 0 0 0 0\n"
"MouseMoveEvent 304 221 0 0 0 0 0\n"
"MouseMoveEvent 304 220 0 0 0 0 0\n"
"MouseMoveEvent 305 219 0 0 0 0 0\n"
"MouseMoveEvent 306 219 0 0 0 0 0\n"
"MouseMoveEvent 307 219 0 0 0 0 0\n"
"MouseMoveEvent 308 219 0 0 0 0 0\n"
"MouseMoveEvent 310 221 0 0 0 0 0\n"
"MouseMoveEvent 311 223 0 0 0 0 0\n"
"MouseMoveEvent 312 226 0 0 0 0 0\n"
"MouseMoveEvent 314 230 0 0 0 0 0\n"
"MouseMoveEvent 315 235 0 0 0 0 0\n"
"MouseMoveEvent 317 242 0 0 0 0 0\n"
"MouseMoveEvent 319 248 0 0 0 0 0\n"
"MouseMoveEvent 321 257 0 0 0 0 0\n"
"MouseMoveEvent 323 266 0 0 0 0 0\n"
"MouseMoveEvent 325 276 0 0 0 0 0\n"
"MouseMoveEvent 327 285 0 0 0 0 0\n"
"MouseMoveEvent 329 292 0 0 0 0 0\n"
"MouseMoveEvent 331 301 0 0 0 0 0\n"
"MouseMoveEvent 333 311 0 0 0 0 0\n"
"MouseMoveEvent 336 319 0 0 0 0 0\n"
"MouseMoveEvent 338 327 0 0 0 0 0\n"
"MouseMoveEvent 341 333 0 0 0 0 0\n"
"MouseMoveEvent 344 341 0 0 0 0 0\n"
"MouseMoveEvent 346 346 0 0 0 0 0\n"
"MouseMoveEvent 348 350 0 0 0 0 0\n"
"MouseMoveEvent 349 355 0 0 0 0 0\n"
"MouseMoveEvent 349 356 0 0 0 0 0\n"
"MouseMoveEvent 350 357 0 0 0 0 0\n"
"MouseMoveEvent 350 359 0 0 0 0 0\n"
"MouseMoveEvent 351 360 0 0 0 0 0\n"
"MouseMoveEvent 351 362 0 0 0 0 0\n"
"MouseMoveEvent 352 363 0 0 0 0 0\n"
"MouseMoveEvent 353 365 0 0 0 0 0\n"
"MouseMoveEvent 354 367 0 0 0 0 0\n"
"MouseMoveEvent 355 369 0 0 0 0 0\n"
"MouseMoveEvent 356 370 0 0 0 0 0\n"
"MouseMoveEvent 357 372 0 0 0 0 0\n"
"MouseMoveEvent 358 374 0 0 0 0 0\n"
"MouseMoveEvent 358 375 0 0 0 0 0\n"
"MouseMoveEvent 360 377 0 0 0 0 0\n"
"MouseMoveEvent 360 378 0 0 0 0 0\n"
"MouseMoveEvent 361 381 0 0 0 0 0\n"
"MouseMoveEvent 361 382 0 0 0 0 0\n"
"MouseMoveEvent 362 384 0 0 0 0 0\n"
"MouseMoveEvent 363 385 0 0 0 0 0\n"
"MouseMoveEvent 364 387 0 0 0 0 0\n"
"MouseMoveEvent 365 389 0 0 0 0 0\n"
"MouseMoveEvent 366 390 0 0 0 0 0\n"
"MouseMoveEvent 368 393 0 0 0 0 0\n"
"MouseMoveEvent 369 395 0 0 0 0 0\n"
"MouseMoveEvent 370 397 0 0 0 0 0\n"
"MouseMoveEvent 371 399 0 0 0 0 0\n"
"LeaveEvent 372 401 0 0 0 0 0\n"
"ExitEvent 372 401 0 0 0 0 0\n"
"EnterEvent 381 393 0 0 0 0 0\n"
"MouseMoveEvent 381 393 0 0 0 0 0\n"
"MouseMoveEvent 379 383 0 0 0 0 0\n"
"MouseMoveEvent 379 371 0 0 0 0 0\n"
"MouseMoveEvent 379 355 0 0 0 0 0\n"
"MouseMoveEvent 379 338 0 0 0 0 0\n"
"MouseMoveEvent 381 319 0 0 0 0 0\n"
"MouseMoveEvent 388 296 0 0 0 0 0\n"
"MouseMoveEvent 399 271 0 0 0 0 0\n"
"LeaveEvent 410 246 0 0 0 0 0\n"
;

int TestSmartVolumeMapperWindowLevel(int argc,
                                     char *argv[])
{
  cout << "CTEST_FULL_OUTPUT (Avoid ctest truncation of output)" << endl;

  vtkNew<vtkRenderWindow> renWin;
  renWin->SetSize(400, 401);
  renWin->SetMultiSamples(0);

  vtkNew<vtkRenderer> ren1;
  ren1->SetViewport(0.0, 0.0, 0.33, 1.0);
  renWin->AddRenderer(ren1.GetPointer());
  vtkNew<vtkRenderer> ren2;
  ren2->SetViewport(0.33, 0.0, 0.66, 1.0);
  renWin->AddRenderer(ren2.GetPointer());
  ren2->SetActiveCamera(ren1->GetActiveCamera());
  vtkNew<vtkRenderer> ren3;
  ren3->SetViewport(0.66, 0.0, 1.0, 1.0);
  renWin->AddRenderer(ren3.GetPointer());
  ren3->SetActiveCamera(ren1->GetActiveCamera());


  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(renWin.GetPointer());

  vtkNew<vtkInteractorStyleTrackballCamera> style;
  iren->SetInteractorStyle(style.GetPointer());

  char* fname = vtkTestUtilities::ExpandDataFileName(argc, argv,
                                                     "Data/HeadMRVolume.mhd");

  vtkNew<vtkMetaImageReader> reader;
  reader->SetFileName(fname);
  reader->Update();
  delete [] fname;

  vtkNew<vtkSmartVolumeMapper> mapper1;
  mapper1->SetInputConnection(reader->GetOutputPort());
  mapper1->SetRequestedRenderModeToDefault();
  vtkNew<vtkSmartVolumeMapper> mapper2;
  mapper2->SetInputConnection(reader->GetOutputPort());
  vtkNew<vtkSmartVolumeMapper> mapper3;
  mapper3->SetInputConnection(reader->GetOutputPort());
  mapper3->SetRequestedRenderModeToRayCast();

  vtkNew<vtkColorTransferFunction> ctf;
  ctf->AddHSVPoint(1.0, 0.65, 1.0, 1.0);
  ctf->AddHSVPoint(256, 0.95, 0.098, 1.0);

  vtkNew<vtkPiecewiseFunction> pwf;
  pwf->AddPoint(1.0, 0.0);
  pwf->AddPoint(255.0, 1.0);

  vtkNew<vtkVolumeProperty> volumeProperty;
  volumeProperty->SetScalarOpacity(pwf.GetPointer());
  volumeProperty->SetColor(ctf.GetPointer());

  // Make sure a context exists
  renWin->Render();

  mapper1->SetBlendModeToMaximumIntensity();
  mapper2->SetBlendModeToMaximumIntensity();
  mapper2->SetFinalColorWindow(0.2);
  mapper2->SetFinalColorLevel(0.75);
  mapper3->SetBlendModeToMaximumIntensity();
  mapper3->SetFinalColorWindow(0.2);
  mapper3->SetFinalColorLevel(0.75);

  vtkNew<vtkVolume> volume1;
  volume1->SetMapper(mapper1.GetPointer());
  volume1->SetProperty(volumeProperty.GetPointer());
  ren1->AddVolume(volume1.GetPointer());
  vtkNew<vtkVolume> volume2;
  volume2->SetMapper(mapper2.GetPointer());
  volume2->SetProperty(volumeProperty.GetPointer());
  ren2->AddVolume(volume2.GetPointer());
  vtkNew<vtkVolume> volume3;
  volume3->SetMapper(mapper3.GetPointer());
  volume3->SetProperty(volumeProperty.GetPointer());
  ren3->AddVolume(volume3.GetPointer());

  ren1->ResetCamera();
  ren2->ResetCamera();
  ren3->ResetCamera();
  renWin->Render();

  return vtkTesting::InteractorEventLoop(argc, argv, iren.GetPointer(),
                                         TestSmartVolumeMapperWindowLevelLog);
}
